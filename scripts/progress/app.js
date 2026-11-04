(() => {
  "use strict";
  const { snapshot: data, history } = JSON.parse(
    document.getElementById("progress-data").textContent,
  );
  const $ = (id) => document.getElementById(id);
  const number = (value) => value.toLocaleString("en-GB");
  const percent = (bytes, total = data.totals.inventory.body_bytes) =>
    total ? (100 * bytes) / total : 0;
  const pct = (value) => `${value.toFixed(2)}%`;
  const labels = {
    source: "Accepted C/legacy",
    generated: "Compiler-generated",
    "lift-blocked": "Lift blocked",
    held: "Held / needs review",
    "c-queue": "Lifted · C queue",
    unlifted: "Unlifted",
  };
  const statePalette = {
    source: "source",
    generated: "generated",
    "lift-blocked": "blocked",
    held: "held",
    "c-queue": "lifted",
    unlifted: "unknown",
  };
  let scope = null,
    page = 0,
    selected = null,
    boxes = [],
    matches = [];
  const canvas = $("treemap"),
    ctx = canvas.getContext("2d");
  let palette;
  const el = (tag, text, className) => {
    const node = document.createElement(tag);
    if (text !== undefined) node.textContent = text;
    if (className) node.className = className;
    return node;
  };
  function updatePalette() {
    const style = getComputedStyle(document.documentElement);
    palette = Object.fromEntries(
      [
        "source",
        "lifted",
        "unknown",
        "held",
        "generated",
        "blocked",
        "text",
        "muted",
        "inset",
        "border",
        "panel",
        "grid",
      ].map((key) => [key, style.getPropertyValue(`--${key}`).trim()]),
    );
  }
  function color(f) {
    const mode = $("color").value;
    if (mode === "state") return palette[statePalette[f.state]];
    if (mode === "recovered")
      return f.source
        ? palette.source
        : f.lifted
          ? palette.lifted
          : palette.unknown;
    return (mode === "source" ? f.source : f[mode])
      ? palette.source
      : palette.unknown;
  }
  function groupName(f) {
    return $("group").value === "module" ? f.module : `Shard ${f.shard} / 8`;
  }
  function metrics() {
    for (const [key, label] of [
      ["source", "Accepted C / legacy"],
      ["recovered", "Lift-inclusive recovery"],
      ["composed", "Composed calls reported"],
      ["integrated", "Live integration recorded"],
    ]) {
      const value = data.totals[key],
        card = el("div", undefined, "metric");
      card.append(
        el("div", label, "metric-label"),
        el("div", pct(percent(value.body_bytes)), "metric-value"),
      );
      const meter = el("div", undefined, "meter"),
        fill = el("span");
      fill.style.width = `${percent(value.body_bytes)}%`;
      meter.append(fill);
      card.append(
        meter,
        el(
          "small",
          `${number(value.functions)} functions · ${number(value.body_bytes)} bytes`,
        ),
      );
      $("metrics").append(card);
    }
    $("stamp").append(
      el(
        "strong",
        `Snapshot · ${new Date(data.generated_at).toLocaleString("en-GB")}`,
      ),
      el(
        "span",
        `Checkout ${data.revision?.slice(0, 8) || "unknown"} · local state included`,
      ),
    );
    const totals = data.totals;
    for (const [label, value] of [
      [
        "Inventory denominator",
        `${number(totals.inventory.functions)} functions / ${number(totals.inventory.body_bytes)} B`,
      ],
      [
        "Accepted C tier",
        `${number(totals.c.functions)} functions / ${number(totals.c.body_bytes)} B`,
      ],
      [
        "Legacy source",
        `${number(totals.legacy.functions)} functions / ${number(totals.legacy.body_bytes)} B`,
      ],
      [
        "Outside inventory · excluded from bytes",
        `${number(Object.keys(data.outside_inventory).length)} entries`,
      ],
      [
        "Zero-byte entries · available in index",
        `${number(data.functions.filter((f) => !f.bytes).length)} functions`,
      ],
    ]) {
      const row = el("div", undefined, "account-row");
      row.append(el("span", label), el("strong", value));
      $("accounting").append(row);
    }
    $("provenance").textContent =
      `Binary SHA-256 ${data.input_sha256}. Addresses: ${data.address_convention}. All percentages use pinned inventory body bytes. Composition counts use current passing experimental reports; integration uses recorded flags. Module grouping uses candidate paths, not proven original source ownership. History starts with the first export and includes changed snapshots only.`;
  }
  // Recursive balanced partition: exact area weights, O(n log n), no dependency
  // on a hosted library. Stable ID tie-breaking keeps equal snapshots stable.
  function partition(items, x, y, width, height, emit) {
    if (!items.length || width <= 0 || height <= 0) return;
    if (items.length === 1) {
      emit(items[0], x, y, width, height);
      return;
    }
    const total = items.reduce((sum, item) => sum + item.weight, 0);
    let index = 1,
      left = items[0].weight;
    while (
      index < items.length - 1 &&
      Math.abs(left + items[index].weight - total / 2) <
        Math.abs(left - total / 2)
    )
      left += items[index++].weight;
    const ratio = left / total;
    if (width >= height) {
      partition(items.slice(0, index), x, y, width * ratio, height, emit);
      partition(
        items.slice(index),
        x + width * ratio,
        y,
        width * (1 - ratio),
        height,
        emit,
      );
    } else {
      partition(items.slice(0, index), x, y, width, height * ratio, emit);
      partition(
        items.slice(index),
        x,
        y + height * ratio,
        width,
        height * (1 - ratio),
        emit,
      );
    }
  }
  function sorted(items) {
    return items.sort(
      (a, b) => b.weight - a.weight || a.id.localeCompare(b.id),
    );
  }
  function draw() {
    updatePalette();
    boxes = [];
    $("tooltip").hidden = true;
    const width = canvas.clientWidth,
      height = canvas.clientHeight,
      dpr = window.devicePixelRatio || 1;
    canvas.width = Math.round(width * dpr);
    canvas.height = Math.round(height * dpr);
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    ctx.fillStyle = palette.inset;
    ctx.fillRect(0, 0, width, height);
    const groups = new Map();
    for (const f of matches.filter((f) => f.bytes > 0)) {
      const key = groupName(f);
      if (!groups.has(key)) groups.set(key, []);
      groups.get(key).push(f);
    }
    const nodes = sorted(
      [...groups].map(([id, functions]) => ({
        id,
        functions,
        weight: functions.reduce((s, f) => s + f.bytes, 0),
      })),
    );
    function drawFunction(item, x, y, w, h) {
      const f = item.function;
      ctx.fillStyle = color(f);
      ctx.fillRect(x, y, Math.max(0, w - 0.6), Math.max(0, h - 0.6));
      if (selected?.id === f.id) {
        ctx.strokeStyle = palette.text;
        ctx.lineWidth = 2;
        ctx.strokeRect(x + 1, y + 1, Math.max(0, w - 2), Math.max(0, h - 2));
      }
      if (scope && w > 100 && h > 24) {
        ctx.fillStyle = palette.text;
        ctx.font = "10px Consolas, monospace";
        ctx.fillText(f.id, x + 5, y + 15, w - 10);
      }
      boxes.push({ f, group: groupName(f), x, y, w, h });
    }
    partition(nodes, 1, 1, width - 2, height - 2, (node, x, y, w, h) => {
      const heading = !scope && w > 90 && h > 55 ? 23 : 0;
      partition(
        sorted(
          node.functions.map((f) => ({
            id: f.id,
            weight: f.bytes,
            function: f,
          })),
        ),
        x + 1,
        y + heading + 1,
        w - 2,
        h - heading - 2,
        drawFunction,
      );
      if (!scope) {
        ctx.strokeStyle = palette.inset;
        ctx.lineWidth = 2;
        ctx.strokeRect(x, y, w, h);
        if (heading) {
          ctx.fillStyle = palette.inset;
          ctx.fillRect(x, y, w, heading);
          ctx.save();
          ctx.beginPath();
          ctx.rect(x, y, w, heading);
          ctx.clip();
          ctx.fillStyle = palette.text;
          ctx.font = "11px Segoe UI, sans-serif";
          ctx.fillText(node.id, x + 6, y + 16);
          ctx.restore();
          boxes.push({ group: node.id, x, y, w, h: heading });
        }
      }
    });
    $("empty").hidden = nodes.length > 0;
    $("empty").textContent = matches.length
      ? "Matching zero-byte entries are listed in the function index."
      : "No functions match this search.";
    drawLegend();
  }
  function drawLegend() {
    $("legend").replaceChildren();
    const mode = $("color").value;
    let values = [
      ["source", mode === "source" ? "Accepted C / legacy" : "Reported"],
      ["unknown", "Remaining inventory"],
    ];
    if (mode === "recovered")
      values = [
        ["source", "Accepted C / legacy"],
        ["lifted", "Accepted lift only"],
        ["unknown", "Remaining inventory"],
      ];
    if (mode === "state")
      values = Object.entries(labels).map(([state, label]) => [
        statePalette[state],
        label,
      ]);
    for (const [key, label] of values) {
      const span = el("span"),
        dot = el("i");
      dot.style.background = palette[key];
      span.append(dot, document.createTextNode(label));
      $("legend").append(span);
    }
  }
  function details(f) {
    selected = f;
    $("details").replaceChildren(
      el("p", "FUNCTION DETAIL", "eyebrow"),
      el("h3", f.id),
      el("p", f.module),
    );
    const list = el("dl");
    for (const [key, value] of [
      ["Original body", `${number(f.bytes)} bytes`],
      ["Work state", labels[f.state]],
      [
        "Accepted source",
        f.source ? (f.source_kind === "legacy" ? "Legacy" : "C tier") : "No",
      ],
      ["Accepted lift", f.lifted ? "Yes" : "No"],
      ["Composed report", f.composed ? "Current pass" : "None reported"],
      ["Live integration", f.integrated ? "Recorded" : "None recorded"],
    ])
      list.append(el("dt", key), el("dd", value));
    $("details").append(
      list,
      el("p", "Inspect the local evidence:"),
      el("code", `py -3.13 -m cmre show ${f.id}`),
    );
  }
  function table() {
    const pages = Math.max(1, Math.ceil(matches.length / 30));
    page = Math.min(page, pages - 1);
    $("functions").replaceChildren();
    for (const f of matches.slice(page * 30, (page + 1) * 30)) {
      const row = el("tr"),
        cell = el("td"),
        button = el("button", f.id);
      button.type = "button";
      button.onclick = () => {
        details(f);
        draw();
      };
      cell.append(button);
      row.append(
        cell,
        el("td", f.module),
        el("td", number(f.bytes)),
        el("td", labels[f.state]),
      );
      $("functions").append(row);
    }
    $("table-count").textContent =
      `${number(matches.length)} matching functions`;
    $("page").textContent = `Page ${page + 1} of ${pages}`;
    $("previous").disabled = page === 0;
    $("next").disabled = page === pages - 1;
  }
  function update() {
    const query = $("search").value.trim().toLowerCase();
    matches = data.functions.filter(
      (f) =>
        (!scope || groupName(f) === scope) &&
        `${f.id} ${f.id.slice(3)} 0x${f.id.slice(3)} ${f.module} ${labels[f.state]}`
          .toLowerCase()
          .includes(query),
    );
    $("scope-count").textContent =
      `${number(matches.length)} functions · ${number(matches.reduce((s, f) => s + f.bytes, 0))} bytes`;
    $("breadcrumb").textContent =
      scope ||
      ($("group").value === "module"
        ? "All candidate modules"
        : "All recovery shards");
    $("back").hidden = !scope;
    table();
    draw();
  }
  function historyChart() {
    const pins = JSON.stringify(Object.entries(data.inventory_sha256).sort());
    const points = history.filter(
      (p) =>
        p.input_sha256 === data.input_sha256 &&
        JSON.stringify(Object.entries(p.inventory_sha256).sort()) === pins,
    );
    const previous = points.length > 1 ? points[points.length - 2] : null;
    $("history-note").textContent = previous
      ? `${number(data.totals.source.body_bytes - previous.totals.source.body_bytes)} accepted source bytes since previous changed snapshot · ${points.length} snapshots on this inventory`
      : "First snapshot saved. Future progress updates will build this history.";
    const svg = $("history-chart"),
      ns = "http://www.w3.org/2000/svg";
    const add = (tag, attrs, text) => {
      const node = document.createElementNS(ns, tag);
      for (const [key, value] of Object.entries(attrs))
        node.setAttribute(key, value);
      if (text) node.textContent = text;
      svg.append(node);
      return node;
    };
    const start = points.length ? Date.parse(points[0].generated_at) : 0,
      end = points.length ? Date.parse(points.at(-1).generated_at) : 0;
    const x = (p) =>
      end === start
        ? 48
        : 48 + ((Date.parse(p.generated_at) - start) / (end - start)) * 560;
    const y = (p, key) =>
      146 -
      percent(p.totals[key].body_bytes, p.totals.inventory.body_bytes) * 1.2;
    for (const value of [0, 25, 50, 75, 100]) {
      const lineY = 146 - value * 1.2;
      add("line", {
        x1: 48,
        x2: 608,
        y1: lineY,
        y2: lineY,
        stroke: palette.grid,
      });
      add(
        "text",
        {
          x: 38,
          y: lineY + 4,
          fill: palette.muted,
          "font-size": 10,
          "text-anchor": "end",
        },
        `${value}%`,
      );
    }
    for (const [key, paint] of [
      ["recovered", palette.lifted],
      ["source", palette.source],
    ]) {
      add("polyline", {
        points: points.map((p) => `${x(p)},${y(p, key)}`).join(" "),
        fill: "none",
        stroke: paint,
        "stroke-width": 2,
      });
      for (const p of points) {
        const dot = add("circle", {
          cx: x(p),
          cy: y(p, key),
          r: 3,
          fill: paint,
        });
        const title = document.createElementNS(ns, "title");
        title.textContent = `${p.generated_at}: ${pct(percent(p.totals[key].body_bytes, p.totals.inventory.body_bytes))}`;
        dot.append(title);
      }
    }
    if (points.length) {
      add(
        "text",
        { x: 48, y: 172, fill: palette.muted, "font-size": 10 },
        new Date(start).toLocaleDateString("en-GB"),
      );
      if (points.length > 1)
        add(
          "text",
          {
            x: 608,
            y: 172,
            fill: palette.muted,
            "font-size": 10,
            "text-anchor": "end",
          },
          new Date(end).toLocaleDateString("en-GB"),
        );
    }
  }
  function hit(event) {
    const r = canvas.getBoundingClientRect(),
      x = event.clientX - r.left,
      y = event.clientY - r.top;
    return boxes.find(
      (b) => x >= b.x && x < b.x + b.w && y >= b.y && y < b.y + b.h,
    );
  }
  canvas.addEventListener("mousemove", (event) => {
    const box = hit(event),
      tip = $("tooltip");
    tip.hidden = !box;
    if (!box) return;
    tip.textContent = box.f
      ? `${box.f.id} · ${number(box.f.bytes)} B\n${box.group}\n${labels[box.f.state]}`
      : `${box.group}\nClick to explore`;
    const rect = canvas.getBoundingClientRect();
    tip.style.left = `${Math.max(0, Math.min(event.clientX - rect.left + 14, rect.width - 270))}px`;
    tip.style.top = `${Math.max(0, Math.min(event.clientY - rect.top + 14, rect.height - 105))}px`;
  });
  canvas.addEventListener("mouseleave", () => {
    $("tooltip").hidden = true;
  });
  canvas.addEventListener("click", (event) => {
    const box = hit(event);
    if (!box) return;
    if (!scope) {
      scope = box.group;
      page = 0;
      update();
    } else if (box.f) {
      details(box.f);
      draw();
    }
  });
  $("search").addEventListener("input", () => {
    page = 0;
    update();
  });
  $("color").addEventListener("change", draw);
  $("group").addEventListener("change", () => {
    scope = null;
    page = 0;
    update();
  });
  $("back").onclick = () => {
    scope = null;
    page = 0;
    update();
  };
  $("reset").onclick = () => {
    scope = null;
    page = 0;
    $("search").value = "";
    update();
  };
  $("previous").onclick = () => {
    page--;
    table();
  };
  $("next").onclick = () => {
    page++;
    table();
  };
  $("theme").onclick = () => {
    const light = document.documentElement.dataset.theme !== "light";
    document.documentElement.dataset.theme = light ? "light" : "dark";
    $("theme").textContent = light ? "Dark theme" : "Light theme";
    update();
    $("history-chart").replaceChildren();
    historyChart();
  };
  new ResizeObserver(draw).observe(canvas.parentElement);
  updatePalette();
  metrics();
  update();
  historyChart();
})();
