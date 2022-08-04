class GraphNode {
  static radius = 5;
  static headerPadding = 8;
  static rowHorizontalPadding = 12;
  static rowVerticalPadding = 6;
  static keyValueSpace = 12;
  static dividerHeight = 2;

  /**
   *
   * @param {{
   *   ctx: d3.Selection;
   *   title: any;
   *   items: { key: any, value: any }[]
   *   color: string
   * }} param0
   */
  constructor({ ctx, title, items, color }) {
    this.x = 0;
    this.y = 0;
    this.title = title;
    this.items = items;
    this.color = color;

    /** @type {number[]} */
    this.rowAnchors = [];
    this.headerHeight = 0;

    this.root = ctx.append("g").attr("class", "graph-node");
    this.background = this.root.append("rect").attr("class", "background");
    this.headerBackground = this.root.append("path");
    this.headerText = this.root.append("text").text(`${this.title}`).attr("class", "header-text");

    /**
     * @type {{
     *   root: d3.Selection;
     *   key: d3.Selection;
     *   value: d3.Selection;
     *   divider?: d3.Selection;
     * }[]}
     */
    this.rows = items.map(({ key, value }, n) => {
      const root = this.root.append("g").attr("class", "row");
      return {
        root,
        key: root.append("text").text(`${key}`).attr("class", "key"),
        value: root.append("text").text(`${value}`).attr("class", "value"),
        divider: n === items.length - 1 ? null : root.append("rect").attr("class", "divider"),
      };
    });

    this.render();
    this.updateLinkPoints();
  }

  /**
   *
   * @param {number} width
   * @param {number} height
   * @param {boolean} isHeaderOnly
   */
  headerPath(width, height, isHeaderOnly) {
    const path = d3.path();
    path.moveTo(0, GraphNode.radius);
    path.arcTo(0, 0, GraphNode.radius, 0, GraphNode.radius);
    path.lineTo(width - GraphNode.radius, 0);
    path.arcTo(width, 0, width, GraphNode.radius, GraphNode.radius);
    if (isHeaderOnly) {
      path.lineTo(width, height - GraphNode.radius);
      path.arcTo(width, height, width - GraphNode.radius, height, GraphNode.radius);
      path.lineTo(GraphNode.radius, height);
      path.arcTo(0, height, 0, height - GraphNode.radius, GraphNode.radius);
    } else {
      path.lineTo(width, height);
      path.lineTo(0, height);
    }
    path.closePath();
    return path.toString();
  }

  render() {
    this.headerText.attr("x", GraphNode.headerPadding).attr("y", GraphNode.headerPadding);

    this.headerHeight = this.headerText.node().getBBox().height + 2 * GraphNode.headerPadding;

    let maxKeyWidth = 0;
    let maxValueWidth = 0;
    let rowY = this.headerHeight;
    this.rows.forEach((it) => {
      it.root.attr("transform", `translate(0, ${rowY})`);
      it.key.attr("x", GraphNode.rowHorizontalPadding);

      const { width: keyWidth, height: keyHeight } = it.key.node().getBBox();
      const { width: valueWidth, height: valueHeight } = it.value.node().getBBox();
      const rowHeight = Math.max(keyHeight, valueHeight) + 2 * GraphNode.rowVerticalPadding;

      it.key.attr("y", (rowHeight - keyHeight) / 2);
      it.value.attr("y", (rowHeight - valueHeight) / 2);
      it.divider?.attr("height", GraphNode.dividerHeight)?.attr("y", rowHeight);

      if (keyWidth > maxKeyWidth) maxKeyWidth = keyWidth;
      if (valueWidth > maxValueWidth) maxValueWidth = valueWidth;

      this.rowAnchors.push(rowY + rowHeight / 2);
      rowY += rowHeight + GraphNode.dividerHeight;
    });

    const maxRowWidth =
      this.rows.length > 0
        ? maxKeyWidth + GraphNode.keyValueSpace + maxValueWidth + 2 * GraphNode.rowHorizontalPadding
        : 0;
    this.width = Math.max(
      this.headerText.node().getBBox().width + 2 * GraphNode.headerPadding,
      maxRowWidth
    );

    this.rows.forEach((it) => {
      it.value.attr("x", maxKeyWidth + GraphNode.keyValueSpace + GraphNode.rowHorizontalPadding);
      it.divider?.attr("width", this.width);
    });

    this.headerBackground
      .attr("d", this.headerPath(this.width, this.headerHeight, this.rows.length === 0))
      .attr("fill", this.color);

    const lastPadding = this.rows.length > 0 ? GraphNode.rowVerticalPadding : 0;
    this.height = this.root.node().getBBox().height + lastPadding;
    this.background.attr("width", this.width).attr("height", this.height);
  }

  updateLinkPoints() {
    /** @type {[number, number]} */
    this.input = [this.x, this.y + this.headerHeight / 2];
    /** @type {[number, number][]} */
    this.outputs = this.rowAnchors.map((it) => [this.x + this.width, this.y + it]);
  }

  /**
   *
   * @param {number} x
   * @param {number} y
   */
  translate(x, y) {
    this.x = x;
    this.y = y;
    this.root.attr("transform", `translate(${this.x}, ${this.y})`);
    this.updateLinkPoints();
  }
}
