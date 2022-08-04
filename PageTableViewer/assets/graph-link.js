class GraphLink {
  static linkPath = d3.linkHorizontal();

  /**
   * @type {GraphLink[]}
   */
  static selected = [];

  /**
   *
   * @param {{
   *   ctx: d3.Selection
   *   source: { node: GraphNode, outputIndex: number }
   *   target: { node: GraphNode }
   * }} param0
   */
  constructor({ ctx, source, target }) {
    this.source = source;
    this.target = target;
    this._isSelected = false;

    this.root = ctx.append("g").attr("class", "graph-link");
    this.line = this.root.append("path").attr("class", "line");
    this.sourcePoint = this.root.append("circle").attr("class", "point");
    this.targetPoint = this.root.append("circle").attr("class", "point");

    const handleClick = () => {
      this.isSelected = !this.isSelected;
      if (this.isSelected) {
        GraphLink.selected.push(this);
      } else {
        const index = GraphLink.selected.indexOf(this);
        if (index !== -1) {
          GraphLink.selected.splice(index, 1);
        }
      }
    };
    this.sourcePoint.on("click", handleClick);
    this.targetPoint.on("click", handleClick);
  }

  get isSelected() {
    return this._isSelected;
  }

  set isSelected(value) {
    this._isSelected = value;
    if (value) {
      this.root.attr("class", "graph-link selected");
    } else {
      this.root.attr("class", "graph-link");
    }
  }

  render() {
    const source = this.source.node.outputs[this.source.outputIndex];
    const target = this.target.node.input;
    this.line.attr("d", GraphLink.linkPath({ source, target }));
    this.sourcePoint.attr("transform", `translate(${source[0]},${source[1]})`);
    this.targetPoint.attr("transform", `translate(${target[0]},${target[1]})`);
  }

  static deselectAll() {
    GraphLink.selected.forEach((it) => (it.isSelected = false));
    GraphLink.selected = [];
  }
}
