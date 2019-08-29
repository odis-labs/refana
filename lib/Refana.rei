
[@ocaml.text
{|Grafana data model.

  This module contains type definitions and construction functions for Grafana
  dashboards.|}]


[@ocaml.text "{1 Positioning}"]



type grid_pos;

let size: (~width: int, ~height: int) => grid_pos;
/** Construct a grid position from a size. */


type xaxis;
let xaxis: (
  ~show: bool=?, ~buckets: option(int)=?, ~mode: string=?,
  ~name: string=?, ~values: list(string)=?, unit
) => xaxis;


type yaxis;
let yaxis: (
  ~format: string=?, ~label: string=?, ~scale: int=?,
  ~max: option(int)=?, ~min: option(int)=?, ~show: bool=?, unit
) => yaxis;


type yaxes;
let yaxes: (~align: bool=?, ~align_level: option(int)=?, unit) => yaxes;


type legend;
let legend: (
  ~avg: bool=?, ~current: bool=?, ~max: bool=?, ~min: bool=?,
  ~show: bool=?, ~total: bool=?, ~values: bool=?, unit
) => legend;


type tooltip;
let tooltip: (~shared: bool=?, ~sort: int=?, ~value_type: string=?, unit) => tooltip;

[@ocaml.text "{1 Queries}"];

module Prometheus_query: {
  type t;

  let make: (
    ~expr: string, ~format: string, ~hide: bool=?, ~interval_factor: int=?,
    ~legend: string=?, ~id: string, ~step: option(int)=?, unit
  ) => t;
};

module Aws_query: {
  type dimensions = list((string, string));

  type t;
  let make: (
    ~id: string, ~namespace: string, ~metric_name: string,
    ~statistics: list(string)=?, ~dimensions: dimensions=?, ~period: string=?,
    ~region: string=?, ~high_resolution: bool=?, ~alias: string=?, unit
  ) => t;
};

module Query: {
  type t =
    | Prometheus_query(Prometheus_query.t)
    | Aws_query(Aws_query.t);

  let prometheus: (Prometheus_query.t) => t;
  let aws: (Aws_query.t) => t;
};


[@ocaml.text "{1 Panels}"];


module Graph: {
  type t;

  let make: (
    ~bars: bool=?, ~dash_length: int=?, ~dashes: bool=?, ~datasource: string=?,
    ~fill: int=?, ~lines: bool=?, ~line_width: int=?, ~percentage: bool=?,
    ~point_radius: int=?, ~points: bool=?, ~space_length: int=?, ~stack: bool=?,
    ~stepped_line: bool=?, ~queries: list(Query.t)=?, ~legend: legend=?,
    ~tooltip: tooltip=?, ~xaxis: xaxis=?, ~yaxes: list(yaxis)=?,
    ~yaxis: yaxis=?, ~position: grid_pos, ~type_: string=?,
    ~id: int=?, ~title: string, unit
  ) => t;
};


module Row: {
  type t;

  let make: (
    ~id: int=?, ~title: string, ~collapsed: bool=?, ~position: grid_pos,
    ~type_: string=?, unit
  ) => t;
};


module Singlestat: {
  type t;

  type value_map = {
    value: string,
    op: string,
    text: string,
  };

  type range_map = {
    from: string,
    to_: string,
    text: string,
  };

  let make: (
    ~title: string,
    ~position: grid_pos,
    ~format: string=?,
    ~description: string=?,
    ~interval: option(string)=?,
    ~datasource: option(string)=?,
    ~span: option(float)=?,
    ~min_span: option(float)=?,
    ~decimals: option(float)=?,
    ~value_name: string=?,
    ~value_font_size: string=?,
    ~prefix_font_size: string=?,
    ~postfix_font_size: string=?,
    ~mapping_type: int=?,
    ~repeat: option(string)=?,
    ~repeat_direction: option(string)=?,
    ~prefix: string=?,
    ~postfix: string=?,
    ~colors: list(string)=?,
    ~color_background: bool=?,
    ~color_value: bool=?,
    ~thresholds: string=?,
    ~value_maps: list(value_map)=?,
    ~range_maps: list(range_map)=?,
    ~transparent: bool=?,
    ~sparkline_fill_color: string=?,
    ~sparkline_full: bool=?,
    ~sparkline_line_color: string=?,
    ~sparkline_show: bool=?,
    ~gauge_show: bool=?,
    ~gauge_min_value: float=?,
    ~gauge_max_value: float=?,
    ~gauge_threshold_markers: bool=?,
    ~gauge_threshold_labels: bool=?,
    unit
  ) => t;
};


module Panel: {
  type t =
    | Row(Row.t)
    | Graph(Graph.t)
    | Singlestat(Singlestat.t);

  let row: Row.t => t;
  let graph: Graph.t => t;
  let singlestat: Singlestat.t => t;
};

[@ocaml.text "{1 Dashboard}"];

/** Dashboard templates and variables.
 
    - {{:https://grafana.com/docs/reference/templating} Variables – Grafana Reference}
 */
module Template: {
  type t;

  module Option: {
    type t;
    let make: (
      ~tags: list(string)=?,
      ~text: string,
      ~value: string,
      ~selected: bool=?,
      unit
    ) => t;
  };

  let make: (
    ~name: string,
    ~datasource: string,
    ~query: string,
    ~label: option(string)=?,
    ~all_value: option(string)=?,
    ~tag_values_query: string=?,
    ~current: option(Option.t)=?,
    ~options: list(Option.t)=?,
    ~hide: int=?,
    ~regex: string=?,
    ~refresh: int=?,
    ~include_all: bool=?,
    ~multi: bool=?,
    ~sort: int=?,
    ~kind: string=?,
    ~tags: list(string)=?,
    ~tagsQuery: string=?,
    ~use_tags: bool=?,
    unit
  ) => t;
};

module Dashboard: {
  type annotation;
  let annotation: (
    ~built_in: int=?, ~datasource: string=?, ~enable: bool=?,
    ~hide: bool=?, ~icon_color: string=?, ~name: string=?,
    ~kind: string=?, unit
  ) => annotation;


  type timepicker;
  let timepicker: (
    ~refresh_intervals: list(string)=?, ~time_options: list(string)=?, unit
  ) => timepicker;


  type interval;
  let interval: (~from: string=?, ~until: string=?, unit) => interval;

  type t;
  let make: (
    ~editable: bool=?,
    ~gnet_id: option(int)=?,
    ~graph_tooltip: int=?,
    ~links: list(string)=?,
    ~panels: list(Panel.t)=?,
    ~schema_version: int=?,
    ~style: string=?,
    ~tags: list(string)=?,
    ~time: interval=?,
    ~timepicker: timepicker=?,
    ~timezone: string=?,
    ~templates: list(Template.t)=?,
    ~title: string,
    ~id: option(int)=?,
    ~uid: option(string)=?,
    ~version: int=?,
    unit
  ) => t;

  let to_yojson: t => Yojson.Safe.t;
};

/** Produces a list of automatically positioned palels.
    The input is a list of rows, where each row can contain mutliple horizontal panels. */
let layout: list(list(Panel.t)) => list(Panel.t);
