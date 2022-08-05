/**
 *
 * @typedef {{
 *   title?: any,
 *   items?: ([any, any]|[any, any, Data])[]
 * }} Data
 */

/**
 *
 * @param {d3.Selection} ctx
 * @param {Data} data
 * @param {string[]} colors
 * @returns {[GraphNode[][], GraphLink[]]}
 */
function parseData(ctx, data, colors) {
  /**
   * @type {GraphNode[][]}
   */
  const nodes = [];
  /**
   * @type {GraphLink[]}
   */
  const links = [];

  /**
   *
   * @param {Data} data
   * @param {number} depth
   */
  function _parseData(data, depth) {
    const items = data.items?.map(([key, value]) => ({ key, value })) || [];
    const node = new GraphNode({ ctx, title: data.title, items, color: colors[depth] || "#000" });
    nodes[depth] ||= [];
    nodes[depth].push(node);

    data.items?.forEach(([_, __, child], n) => {
      if (child) {
        const childNode = _parseData(child, depth + 1);
        links.push(
          new GraphLink({
            ctx,
            source: { node, outputIndex: n },
            target: { node: childNode },
          })
        );
      }
    });

    return node;
  }

  _parseData(data, 0);
  return [nodes, links];
}
/**
 *
 * @param {GraphNode[][]} nodes
 * @param {number} paddingX
 * @param {number} paddingY
 */
function layoutNodes(nodes, paddingX, paddingY) {
  let currentX = 0;
  let currentY = 0;
  nodes.forEach((them) => {
    const height = them.reduce((s, it) => it.height + s, 0) + paddingY * (them.length - 1);
    const dynamicPaddingX = height / 4;
    currentX += dynamicPaddingX > paddingX ? dynamicPaddingX : paddingX;
    currentY = (innerHeight - height) / 2;
    let maxWidth = 0;
    them.forEach((it) => {
      it.translate(currentX, currentY);
      currentY += it.height + paddingY;
      if (it.width > maxWidth) maxWidth = it.width;
    });
    currentX += maxWidth;
  });
}

/**
 *
 * @param {Data} data
 */
function main(data) {
  const svg = d3.select("svg").attr("width", innerWidth).attr("height", innerHeight);
  const root = svg.append("g");
  const zoom = d3
    .zoom()
    .on("zoom", (e) => root.attr("transform", e.transform))
    .filter(({ ctrlKey, type, button }) => !ctrlKey && (type === "wheel" || button === 1));
  svg.call(zoom).on("dblclick.zoom", null);

  d3.select(window)
    .on("keydown", ({ key }) => {
      if (key !== "Delete") return;
      GraphLink.deselectAll();
    })
    .on("resize", () => {
      svg.attr("width", innerWidth).attr("height", innerHeight);
    });

  const [nodes, links] = parseData(root, data, [
    "#5e4fa2",
    "#3288bd",
    "#66c2a5",
    "#fdae61",
    "#d53e4f",
  ]);
  layoutNodes(nodes, 120, 20);
  links.forEach((it) => it.render());
}
