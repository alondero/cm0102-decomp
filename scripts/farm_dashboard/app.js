"use strict";

const byId = id => document.getElementById(id);
const number = new Intl.NumberFormat();
const clock = new Intl.DateTimeFormat(undefined, { hour: "2-digit", minute: "2-digit", second: "2-digit", hourCycle: "h23" });
const hour = new Intl.DateTimeFormat(undefined, { hour: "2-digit", minute: "2-digit", hourCycle: "h23" });
const date = new Intl.DateTimeFormat(undefined, { month: "short", day: "numeric", hour: "2-digit", minute: "2-digit", hourCycle: "h23" });
const count = value => value == null ? "—" : number.format(value);
const bytes = value => {
  if (value == null) return "—";
  if (value < 1024) return count(value) + " B";
  if (value < 1048576) return (value / 1024).toFixed(1) + " KiB";
  return (value / 1048576).toFixed(2) + " MiB";
};
const time = (at, format = clock) => at == null ? "—" : format.format(at * 1000);
const age = (at, now) => {
  if (at == null) return "—";
  const seconds = Math.max(0, Math.floor(now - at));
  if (seconds < 60) return seconds + " s";
  if (seconds < 3600) return Math.floor(seconds / 60) + " min";
  return Math.floor(seconds / 3600) + " h " + Math.floor(seconds % 3600 / 60) + " min";
};

let overview = null;
let inventory = null;
let selected = new URLSearchParams(location.search).get("run") || "";
let runOptions = "";

function el(tag, attrs = {}, ...children) {
  const node = document.createElement(tag);
  for (const [key, value] of Object.entries(attrs)) node.setAttribute(key, value);
  node.append(...children);
  return node;
}

function pill(label, tone = "") {
  return el("span", { class: "pill " + tone }, label);
}

function tone(status) {
  if (status === "live") return "good";
  if (["draining", "stale", "legacy live", "restarting", "unverified"].includes(status)) return "warn";
  if (["interrupted", "crash loop"].includes(status)) return "bad";
  return "";
}

function meter(percent, color) {
  return el("span", { class: "meter", style: "--accent: var(--" + color + ")" },
    el("span", { style: "width: " + Math.max(0, Math.min(100, percent)) + "%" }));
}

function metric(label, value, note, color, percent) {
  const card = el("div", { class: "metric", "data-color": color || "" },
    el("span", { class: "metric-label" }, label),
    el("strong", { class: "metric-value" }, value),
    el("span", { class: "metric-note" }, note));
  if (percent != null) card.append(meter(percent, color));
  return card;
}

function lane(id, title, value, detail) {
  return el("div", { class: "lane", "data-lane": id },
    el("a", { href: id === "supply" ? "#supply" : "#assignments", "data-filter": id }, title),
    el("strong", { class: "lane-value" }, value), el("p", { class: "lane-note" }, detail));
}

function split(run) {
  const jobs = run?.jobs || [];
  const models = jobs.filter(job => job.stage === "model");
  return {
    translation: models.filter(job => job.kind !== "audit"),
    audit: models.filter(job => job.kind === "audit"),
    gate: jobs.filter(job => job.stage === "gating"),
    gateQueue: jobs.filter(job => ["gate complete", "gate queue"].includes(job.stage)),
    accepting: jobs.filter(job => job.stage === "accepting"),
    acceptQueue: jobs.filter(job => ["accept queue", "awaiting writer"].includes(job.stage)),
  };
}

const stopped = run => ["stopped", "interrupted", "crash loop"].includes(run?.status);
const live = run => ["live", "draining"].includes(run?.status);
const capacity = (used, total) => count(used) + (total == null ? "" : " / " + count(total));
const role = job => job.role || ({ translate: "Translation", fix: "Defect fix", audit: job.confirm ? "Confirmation" : "Audit", adopt: "Adoption" }[job.kind] || job.kind);
const titleCase = value => value.charAt(0).toUpperCase() + value.slice(1);

function renderOverview() {
  const run = overview?.run;
  const data = inventory?.coverage;
  const settings = run?.settings || {};
  const queue = run?.scheduler || {};
  const stages = split(run);
  const known = run && (run.telemetry || stopped(run));
  const liveQueue = !stopped(run) && run?.telemetry;
  const supply = overview?.supply || [];
  const sourceMetric = data
    ? metric("Accepted C / legacy", data.source_percent.toFixed(2) + "%",
      count(data.source_functions) + " functions · " + bytes(data.source_body_bytes), "mint", data.source_percent)
    : metric("Accepted C / legacy", "—", "Workspace inventory");
  const liftMetric = data
    ? metric("Lift-inclusive recovery", data.recovered_percent.toFixed(2) + "%",
      count(data.recovered_functions) + " functions · " + bytes(data.recovered_body_bytes), "sky", data.recovered_percent)
    : metric("Lift-inclusive recovery", "—", "Workspace inventory");
  const assigned = stages.translation.length + stages.audit.length;
  byId("metrics").replaceChildren(sourceMetric, liftMetric,
    metric("Accepted in this run", bytes(run?.counts.accepted_bytes), count(run?.counts.accepted_functions) + " distinct functions"),
    metric(live(run) || stopped(run) ? "Active model workers" : "Observed model assignments", run ? count(assigned) : "—",
      count(supply.length) + " separately reported sessions"));

  const frontier = liveQueue ? queue.frontier : null;
  const supplyNote = stopped(run) ? "Farm stopped · last run retained"
    : frontier == null ? "Waiting for a frontier reading"
      : bytes(queue.frontier_bytes) + " in frontier · " + count(queue.pending) + " retries";
  byId("lanes").replaceChildren(
    lane("supply", "Supply", count(frontier), supplyNote),
    lane("translation", "Translate & fix", run ? capacity(stages.translation.length, settings.workers) : "—",
      stopped(run) ? "Translation pool stopped" : queue.translation_model || (known ? "Translation model unavailable" : "Model slots")),
    lane("gate", "Gate re-test", known ? capacity(stages.gate.length, run.verifier_slots) : "—",
      known ? count(stages.gateQueue.length) + " waiting or ready to collect" : "Waiting for gate telemetry"),
    lane("acceptance", "Serial acceptance", known ? capacity(stages.accepting.length, 1) : "—",
      known ? count(stages.acceptQueue.length) + " waiting for acceptance" : "Waiting for acceptance telemetry"),
    lane("audit", "Audit & confirm", run ? capacity(stages.audit.length, settings.audit_workers) : "—",
      liveQueue ? count(queue.audit_queue) + " queued for review" : "Review pool"));
  byId("pipeline-caption").textContent = stopped(run) ? "Stopped run · last applied capacities" : "Selected run · active / configured";
  renderStatus(run);
  renderAssignments();
  renderScaling(run, stages);
  renderThroughput(run);
  renderEvents();
}

function renderStatus(run) {
  const selector = byId("run-select");
  const runs = overview?.runs || [];
  const options = JSON.stringify(runs);
  if (runOptions !== options) {
    selector.replaceChildren(...runs.map(row => el("option", { value: row.id }, row.name + " · " + row.status)));
    if (!runs.length) selector.append(el("option", { value: "" }, "No recorded runs"));
    selector.disabled = !runs.length;
    runOptions = options;
  }
  selector.value = run?.id || "";
  const status = run?.status || (overview ? "no runs" : "connecting");
  const label = byId("status");
  label.textContent = titleCase(status);
  label.className = "pill " + tone(status);
  const descriptions = {
    live: "Farm scheduler is reporting",
    draining: "Farm is finishing in-flight work",
    stopped: "Historical run",
    stale: "Showing last observed assignments",
    "legacy live": "Older farm · dispatch-to-result records only",
    unverified: "Farm process could not be verified",
    interrupted: "Farm exited without a terminal event",
    "crash loop": "Supervisor stopped restarting the farm",
    restarting: "Last supervisor event was a restart",
    "no runs": "Start a farm run to populate the pipeline",
  };
  byId("status-detail").textContent = descriptions[status] || "";
  byId("updated").textContent = run?.last_at ? "Last signal " + time(run.last_at, date) : "";
}

function renderAssignments() {
  const run = overview?.run;
  const jobs = run?.jobs || [];
  const reports = (overview?.supply || []).map(row => ({ ...row, stage: "reported", since: row.reported_at }));
  const filter = byId("assignment-filter").value;
  const modelCount = jobs.filter(job => job.stage === "model").length;
  byId("assignment-summary").textContent = count(modelCount) + " model assignments · " +
    count(jobs.length - modelCount) + " gate or acceptance assignments · " + count(reports.length) + " reported";
  const rows = [...jobs, ...reports].filter(job => filter === "all" || filter === job.stage ||
    (filter === "gate" && !["model", "reported"].includes(job.stage)));
  if (rows.length) {
    const labels = ["Role / stage", "Worker / model", "Assignment", "Age"];
    byId("assignment-rows").replaceChildren(el("table", {},
      el("thead", {}, el("tr", {}, ...labels.map(label => el("th", { scope: "col" }, label)))),
      el("tbody", {}, ...rows.map(job => {
        const stage = job.stage === "model" ? (run.telemetry ? "Model" : "In flight") : titleCase(job.stage);
        const detail = job.module ? job.module.split(/[\\/]/).at(-1) : "";
        const assignment = el("span", { class: "assignment" }, job.fn || job.work);
        if (detail || job.bytes != null) assignment.append(el("span", { class: "subline" },
          [detail, job.bytes != null ? bytes(job.bytes) : ""].filter(Boolean).join(" · ")));
        return el("tr", {},
          el("td", {}, titleCase(role(job)), el("span", { class: "subline" }, stage)),
          el("td", {}, el("span", { class: "assignment" }, job.model || "—"),
            el("span", { class: "subline" }, job.id || ("Job #" + job.n))),
          el("td", {}, assignment),
          el("td", {}, age(job.since, overview.at)));
      }))));
  } else {
    const empty = filter === "reported" ? "No expert sessions have a current report."
      : stopped(run) ? "There are no active farm assignments for this run."
        : "No assignments match this view.";
    byId("assignment-rows").replaceChildren(el("p", { class: "empty" }, empty));
  }
  const models = new Map();
  for (const job of jobs.filter(job => job.stage === "model")) {
    const label = role(job) + " · " + job.model;
    models.set(label, (models.get(label) || 0) + 1);
  }
  byId("pool-models").replaceChildren(...Array.from(models, ([label, total]) => pill(count(total) + " × " + label)));
}

function reading(label, value) {
  return el("div", { class: "reading" }, el("dt", {}, label), el("dd", {}, value));
}

function scalingSignal(run, stages) {
  const queue = run.scheduler;
  const settings = run.settings;
  if (stopped(run)) return ["Historical settings", "Queue and capacity signals resume when the farm runs.", ""];
  if (!live(run)) return ["Live capacity unverified", "Use the last signal time to assess freshness.", "warn"];
  if (run.status === "draining") return ["Draining", "The farm is finishing jobs already in progress.", "warn"];
  if (settings.workers <= 0) return ["Translation paused", "The applied translation worker limit is zero.", "warn"];
  if (!queue.translation_model) return ["Model limit", "No translation ladder model is currently available.", "warn"];
  const backlog = [];
  if (stages.acceptQueue.length) backlog.push(count(stages.acceptQueue.length) + " await serial acceptance");
  if (stages.gateQueue.length) backlog.push(count(stages.gateQueue.length) + " await a gate slot or collection");
  if (queue.audit_queue) backlog.push(count(queue.audit_queue) + " await audit");
  const extra = backlog.length ? " Downstream: " + backlog.join(", ") + "." : "";
  const idle = Math.max(0, settings.workers - stages.translation.length);
  if (idle && queue.supply_empty_since != null && queue.frontier === 0 && queue.pending === 0) {
    return ["Supply constrained", count(idle) + " translation slots idle. The frontier has been empty for " +
      age(queue.supply_empty_since, overview.at) + "." + extra, "warn"];
  }
  if (stages.acceptQueue.length && stages.accepting.length) {
    return ["Serial acceptance queue", count(stages.acceptQueue.length) + " candidates await the writer.", "warn"];
  }
  if (stages.acceptQueue.some(job => job.stage === "awaiting writer")) {
    return ["Acceptance waiting for writer", "The farm will retry when the writer lease is available.", "warn"];
  }
  if (stages.gateQueue.length && stages.gate.length >= run.verifier_slots) {
    return ["Gate re-test queue", "All gate slots are occupied." + extra, "warn"];
  }
  if (queue.frontier > 0 && !idle) return ["Translation pool full", count(queue.frontier) + " functions remain in the frontier." + extra, ""];
  if (queue.pending && idle) return ["Retries pending", count(queue.pending) + " jobs await redispatch. Review provider limits and the ladder.", "warn"];
  return ["Dispatch active", (idle ? count(idle) + " translation slots available." : "All translation slots occupied.") + extra, ""];
}

function renderScaling(run, stages) {
  const target = byId("scaling");
  const detailsOpen = target.querySelector("details")?.open || false;
  if (!run) {
    target.replaceChildren(el("p", {}, "Applied settings appear once a farm run is available."));
    return;
  }
  const queue = run.scheduler || {};
  const settings = run.settings;
  const [headline, explanation, statusTone] = scalingSignal(run, stages);
  const rows = [
    reading("Translation slots", count(settings.workers)),
    reading("Audit slots", count(settings.audit_workers)),
    reading("Gate re-test slots", count(run.verifier_slots)),
    reading("Supply byte range", settings.min_bytes == null ? "—" : bytes(settings.min_bytes) + " – " + (settings.max_bytes == null ? "no ceiling" : bytes(settings.max_bytes))),
    reading("Include held", settings.include_held == null ? "—" : settings.include_held ? "Yes" : "No"),
    reading("Last frontier scan", time(queue.last_scan, date)),
    reading("Next scan eligible", time(queue.next_scan, date)),
    reading("Last settings change", time(run.settings_changed_at, date)),
  ];
  const notices = [];
  if (!stopped(run)) {
    for (const [provider, until] of Object.entries(queue.limited || {})) {
      notices.push(pill(provider + " limited until " + time(until, date), "warn"));
    }
    for (const model of queue.unavailable || []) notices.push(pill(model + " unavailable", "bad"));
  }
  for (const [key, value] of Object.entries(run.requested_settings || {})) {
    const next = Array.isArray(value) ? value.join(", ") : value ?? "—";
    notices.push(pill("Pending · " + key.replaceAll("_", " ") + " → " + next, "warn"));
  }
  const more = el("details", {}, el("summary", {}, "Model ladder and live tuning"));
  more.open = detailsOpen;
  if (settings.ladder?.length) {
    more.append(el("ol", { class: "model-ladder" }, ...settings.ladder.map(model => el("li", {}, model))));
  }
  const quoted = "'" + run.id.replaceAll("'", "''") + "'";
  more.append(el("p", {}, "Run from the checkout. The scheduler reads changes at the start of its next tick."),
    el("pre", {}, "py -3.13 scripts/cmre_farm.py set " + quoted + " --workers " +
      (settings.workers ?? "<count>") + " --audit-workers " + (settings.audit_workers ?? "<count>")));
  target.replaceChildren(
    el("div", { class: "callout " + statusTone }, el("strong", {}, headline), el("p", {}, explanation)),
    el("dl", {}, ...rows),
    ...(notices.length ? [el("div", { class: "provider-list" }, ...notices)] : []), more);
}

function renderInventory() {
  const target = byId("inventory");
  const run = overview?.run;
  const counts = run?.counts || {};
  if (!inventory || inventory.error) {
    target.textContent = inventory?.error || "Reading the local inventory…";
    byId("inventory-age").textContent = "";
    return;
  }
  byId("inventory-age").textContent = "Read " + time(inventory.at);
  const bands = Object.entries(inventory.bands).map(([name, band]) =>
    el("div", { class: "band" }, el("span", {}, titleCase(name)),
      el("strong", {}, bytes(band.bytes)), el("span", {}, count(band.functions) + " functions")));
  const holds = inventory.families.map(row => reading(row.name, bytes(row.bytes) + " · " + count(row.functions) + " fn"));
  const inputs = count(counts.input_needed) + " candidates · " + bytes(counts.input_bytes);
  const summary = el("dl", {}, reading("Input needed in run", inputs),
    reading("Parked in run", count(counts.parked)), reading("Open audit defects in run", count(counts.defects)));
  target.replaceChildren(el("div", { class: "band-grid" }, ...bands), summary,
    el("p", { class: "family-head" }, "Largest recorded blocker families"),
    holds.length ? el("dl", { class: "family-list" }, ...holds) : el("p", {}, "No evidence or harness holds recorded."));
}

function renderThroughput(run) {
  const target = byId("throughput");
  if (!run) {
    target.replaceChildren(el("p", { class: "empty" }, "No run selected."));
    return;
  }
  const max = Math.max(1, ...run.throughput.map(row => row.bytes));
  const bars = run.throughput.map(row => {
    const label = time(row.at, date) + " · " + bytes(row.bytes) + " · " + count(row.functions) + " translations";
    return el("div", { class: "bucket", title: label, "aria-label": label },
      el("div", { class: "bar", "aria-hidden": "true" }, el("i", { style: "height: " + (row.bytes / max * 100) + "%" })),
      el("strong", {}, bytes(row.bytes)), el("time", {}, time(row.at, hour)));
  });
  byId("throughput-caption").textContent = "Last six hour buckets · latest starts " + time(run.throughput.at(-1).at, date);
  target.replaceChildren(el("div", { class: "chart" }, ...bars),
    el("p", { class: "chart-note" }, "Original body bytes from translation acceptances. Fixes and adoptions are excluded."));
}

function eventDetail(event) {
  if (event.changes) {
    return Object.entries(event.changes).map(([key, values]) => key.replaceAll("_", " ") +
      " " + values.map(value => Array.isArray(value) ? value.join(", ") : String(value ?? "—")).join(" → ")).join(" · ");
  }
  return [event.fn, event.confirm ? "confirmation" : event.kind, event.model, event.outcome,
    event.bytes == null ? "" : bytes(event.bytes), event.until ? "until " + time(event.until, date) : "",
    event.idle == null ? "" : count(event.idle) + " idle slots", event.coverage == null ? "" : (event.coverage * 100).toFixed(1) + "% coverage",
    event.dry_minutes == null ? "" : event.dry_minutes + " min empty"].filter(Boolean).join(" · ");
}

function renderEvents() {
  const rows = overview?.run?.activity[byId("event-filter").value] || [];
  const warning = /needed|limited|unavailable|empty|defect|blocked|failed|crash|quarantined/;
  byId("events").replaceChildren(...(rows.length ? rows.map(event =>
    el("div", { class: "event" }, el("time", { title: time(event.at, date) }, time(event.at)),
      el("span", { class: "event-title" + (warning.test(event.event) ? " warn" : "") }, titleCase(event.event.replaceAll("-", " "))),
      el("span", { class: "event-detail" }, eventDetail(event))))
    : [el("p", { class: "empty" }, "No events match this view.")]));
}

async function fetchJson(path) {
  const response = await fetch(path, { signal: AbortSignal.timeout(15000) });
  if (!response.ok) throw new Error("Dashboard request failed");
  return response.json();
}

function connection(ok) {
  const label = byId("connection");
  const status = ok ? "AUTO-REFRESH ON" : "CONNECTION LOST";
  if (label.textContent.includes(status)) return;
  label.className = "connection " + (ok ? "online" : "offline");
  label.replaceChildren(el("i"), " " + status);
}

let refreshTimer;
let requestNumber = 0;
async function refresh() {
  clearTimeout(refreshTimer);
  const request = ++requestNumber;
  try {
    const data = await fetchJson("/api/overview" + (selected ? "?run=" + encodeURIComponent(selected) : ""));
    if (request !== requestNumber) return;
    overview = data;
    renderOverview();
    renderInventory();
    connection(true);
  } catch {
    if (request === requestNumber) connection(false);
  }
  if (request === requestNumber) refreshTimer = setTimeout(refresh, 5000);
}

async function refreshInventory() {
  try {
    inventory = await fetchJson("/api/inventory");
  } catch {
    inventory = { error: "Workspace inventory is unavailable." };
  }
  renderInventory();
  renderOverview();
  setTimeout(refreshInventory, 60000);
}

byId("run-select").addEventListener("change", event => {
  selected = event.target.value;
  history.replaceState(null, "", "?run=" + encodeURIComponent(selected) + location.hash);
  refresh();
});
byId("assignment-filter").addEventListener("change", renderAssignments);
byId("event-filter").addEventListener("change", renderEvents);
byId("lanes").addEventListener("click", event => {
  const lane = event.target.closest("[data-filter]")?.dataset.filter;
  if (!lane || lane === "supply") return;
  byId("assignment-filter").value = ["gate", "acceptance"].includes(lane) ? "gate" : "model";
  renderAssignments();
});
refresh();
refreshInventory();
