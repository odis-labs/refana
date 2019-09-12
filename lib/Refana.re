
let id = {
  let x = ref(1000);
  () => { incr(x); x^ }
};


[@deriving (to_yojson)]
type grid_pos = {
  x: int,
  y: int,
  w: int,
  h: int
};

[@deriving (make, to_yojson)]
type xaxis = {
  [@deriving.make.default true]
  show: bool,

  [@deriving.make.default None]
  buckets: option(int),

  [@deriving.make.default "time"]
  mode: string,

  [@deriving.make.default ""]
  name: string,

  [@deriving.make.default []]
  values: list(string)
};
let xaxis = make_xaxis;


[@deriving (make, to_yojson)]
type yaxis = {
  [@deriving.make.default "short"]
  format: string,

  [@deriving.make.default ""]
  label: string,

  [@deriving.yojson.key "logBase"]
  [@deriving.make.default 1]
  scale: int,

  [@deriving.make.default None]
  max: option(int),

  [@deriving.make.default None]
  min: option(int),

  [@deriving.make.default true]
  show: bool
};
let yaxis = make_yaxis;


[@deriving (make, to_yojson)]
type yaxes = {
  [@deriving.make.default false]
  align: bool,

  [@deriving.yojson.key "alignLevel"]
  [@deriving.make.default None]
  align_level: option(int)
};
let yaxes = make_yaxes;


[@deriving (make, to_yojson { strict: false })]
type legend = {
  [@deriving.make.default false]
  avg: bool,

  [@deriving.make.default false]
  current: bool,

  [@deriving.make.default false]
  max: bool,

  [@deriving.make.default false]
  min: bool,

  [@deriving.make.default true]
  show: bool,

  [@deriving.make.default false]
  total: bool,

  [@deriving.make.default false]
  values: bool
};
let legend = make_legend;


[@deriving (make, to_yojson { strict: false })]
type tooltip = {
  [@deriving.make.default true]
  shared: bool,

  [@deriving.make.default 0]
  sort: int,

  /* Yes, this one uses snake-case, not camel-case. */
  [@deriving.make.default "individual"]
  value_type: string
};
let tooltip = make_tooltip;

module Prometheus_query = {
  [@deriving (make, to_yojson { strict: false })]
  type t = {
    expr: string,

    format: string,

    [@deriving.make.default false]
    hide: bool,

    [@deriving.yojson.key "intervalFactor"]
    [@deriving.make.default 2]
    interval_factor: int,

    [@deriving.make.default ""]
    [@deriving.yojson.key "legendFormat"]
    legend: string,

    [@deriving.yojson.key "refid"]
    id: string,

    [@deriving.make.default None]
    step: option(int)
  };
};

module Aws_query = {
  type dimensions = list((string, string));

  let dimensions_to_yojson = dimensions =>
    `Assoc(List.map(((key, value)) => (key, `String(value)), dimensions));

  [@deriving (make, to_yojson { strict: false })]
  type t = {
    [@deriving.yojson.key "refid"]
    id: string,

    namespace: string,

    [@deriving.yojson.key "metricName"]
    metric_name: string,

    [@deriving.make.default []]
    statistics: list(string),

    [@deriving.make.default []]
    dimensions: dimensions,

    [@deriving.make.default ""]
    period: string,

    [@deriving.make.default ""]
    region: string,

    [@deriving.make.default false]
    high_resolution: bool,

    [@deriving.make.default ""]
    alias: string,
  };
};

module Query = {
  type t =
    | Prometheus_query(Prometheus_query.t)
    | Aws_query(Aws_query.t);

  let prometheus = x => Prometheus_query(x);
  let aws = x => Aws_query(x);

  let to_yojson = (panel) =>
    switch(panel) {
    | Prometheus_query(query) => Prometheus_query.to_yojson(query)
    | Aws_query(query) => Aws_query.to_yojson(query)
    };
};

module Graph = {

  type override = Yojson.Safe.t;

  let override = (~alias, ~color=?, ()) => {
    let items = [
      ("alias", `String(alias))
    ];
    let items =
      switch(color) {
      | Some(color) => [("color", `String(color)), ...items]
      | None => items
      };
    `Assoc(items)
  };
  
  let override_to_yojson = (x) => x;

  [@deriving (make, to_yojson { strict: false })]
  type t = {
    [@deriving.make.default false]
    bars: bool,

    [@deriving.yojson.key "dashLength"]
    [@deriving.make.default 10]
    dash_length: int,

    [@deriving.make.default false]
    dashes: bool,

    [@deriving.make.default "prometheus"]
    datasource: string,

    [@deriving.make.default ""]
    description: string,

    [@deriving.make.default 1]
    fill: int,

    [@deriving.make.default true]
    lines: bool,

    [@deriving.yojson.key "linewidth"]
    [@deriving.make.default 1]
    line_width: int,

    [@deriving.make.default false]
    percentage: bool,

    [@deriving.yojson.key "pointradius"]
    [@deriving.make.default 5]
    point_radius: int,

    [@deriving.make.default false]
    points: bool,

    [@deriving.yojson.key "spaceLength"]
    [@deriving.make.default 10]
    space_length: int,

    [@deriving.yojson.key "nullPointMode"]
    [@deriving.make.default None]
    null_point_mode: option(string),

    [@deriving.make.default false]
    stack: bool,

    [@deriving.yojson.key "steppedLine"]
    [@deriving.make.default false]
    stepped_line: bool,

    [@deriving.yojson.key "seriesOverrides"]
    [@deriving.make.default []]
    series_overrides: list(override),

    [@deriving.yojson.key "targets"]
    [@deriving.make.default []]
    queries: list(Query.t),

    [@deriving.make.default legend()]
    legend: legend,

    [@deriving.make.default tooltip()]
    tooltip,

    [@deriving.make.default xaxis()]
    xaxis: xaxis,

    [@deriving.make.default [yaxis(), yaxis()]]
    yaxes: list(yaxis),

    [@deriving.make.default yaxis()]
    yaxis: yaxis,

    [@deriving.yojson.key "gridPos"]
    position: grid_pos,

    [@deriving.yojson.key "type"]
    [@deriving.make.default "graph"]
    type_ : string,

    [@deriving.make.default id()]
    id: int,

    title: string
  };
};


module Row = {
  [@deriving (make, to_yojson { strict: false })]
  type t = {
    [@deriving.make.default id()]
    id: int,

    title: string,

    [@deriving.make.default false]
    collapsed: bool,

    [@deriving.yojson.key "gridPos"]
    position: grid_pos,

    [@deriving.yojson.key "type"]
    [@deriving.make.default "row"]
    type_ : string
  }
};


module Singlestat = {
  [@deriving (to_yojson { strict: false })]
  type value_map = {
    value: string,
    op: string,
    text: string,
  };

  [@deriving (to_yojson { strict: false })]
  type range_map = {
    from: string,

    [@deriving.yojson.key "to"]
    to_: string,

    text: string,
  };

  module Gauge = {
    [@deriving (make, to_yojson { strict: false })]
    type t = {
      [@deriving.yojson.key "minValue"]
      [@deriving.make.default 0.0]
      min_value: float,

      [@deriving.yojson.key "maxValue"]
      [@deriving.make.default 100.0]
      max_value: float,

      [@deriving.make.default false]
      show: bool,

      [@deriving.yojson.key "thresholdLabels"]
      [@deriving.make.default false]
      threshold_labels: bool,

      [@deriving.yojson.key "thresholdMarkers"]
      [@deriving.make.default true]
      threshold_markers: bool
    };
  };

  module Sparkline = {
    [@deriving (make, to_yojson { strict: false })]
    type t = {
      [@deriving.yojson.key "fillColor"]
      [@deriving.make.default "rgba(31, 118, 189, 0.18)"]
      fill_color: string,

      [@deriving.make.default false]
      full: bool,

      [@deriving.make.default "rgb(31, 120, 193)"]
      lineColor: string,

      [@deriving.make.default true]
      show: bool
    };
  }
  
  module Target = {
    [@deriving (make, to_yojson { strict: false })]
    type t = {
      expr: string,

      [@deriving.make.default "percent"]
      format: string,

      [@deriving.yojson.key "intervalFactor"]
      interval_factor: int,

      [@deriving.yojson.key "refId"]
      ref_id: string
    };
  };

  [@deriving (make, to_yojson { strict: false })]
  type t = {
    title: string,

    [@deriving.yojson.key "gridPos"]
    position: grid_pos,

    [@deriving.make.default id()]
    id: int,

    [@deriving.make.default "none"]
    format: string,

    [@deriving.yojson.key "type"]
    [@deriving.make.default "singlestat"]
    kind: string,

    [@deriving.make.default ""]
    description: string,

    [@deriving.make.default None]
    interval: option(string),

    [@deriving.make.default ""]
    datasource: string,

    [@deriving.make.default None]
    span: option(float),

    [@deriving.make.default None]
    min_span: option(float),

    [@deriving.make.default None]
    decimals: option(float),

    [@deriving.yojson.key "valueName"]
    [@deriving.make.default "avg"]
    value_name: string,

    [@deriving.yojson.key "valueFontSize"]
    [@deriving.make.default "80%"]
    value_font_size: string,

    [@deriving.make.default ""]
    prefix: string,

    [@deriving.make.default ""]
    postfix: string,

    [@deriving.yojson.key "prefixFontSize"]
    [@deriving.make.default "50%"]
    prefix_font_size: string,

    [@deriving.yojson.key "postfixFontSize"]
    [@deriving.make.default "50%"]
    postfix_font_size: string,

    [@deriving.yojson.key "mappingType"]
    [@deriving.make.default 1]
    mapping_type: int,

    [@deriving.yojson.key "maxDataPoints"]
    [@deriving.make.default 100]
    max_data_points: int,

    [@deriving.yojson.key "nullPointMode"]
    [@deriving.make.default "connected"]
    null_point_mode: string,

    [@deriving.make.default ["#299c46", "rgba(237, 129, 40, 0.89)", "#d44a3a"]]
    colors: list(string),

    [@deriving.yojson.key "colorBackground"]
    [@deriving.make.default false]
    color_background: bool,

    [@deriving.yojson.key "colorValue"]
    [@deriving.make.default false]
    color_value: bool,

    [@deriving.make.default ""]
    thresholds: string,

    [@deriving.yojson.key "valueMaps"]
    [@deriving.make.default [{value: "null", op: "=", text: "N/A"}]]
    value_maps: list(value_map),

    [@deriving.yojson.key "rangeMaps"]
    [@deriving.make.default [{from: "null", to_: "null", text: "N/A"}]]
    range_maps: list(range_map),

    [@deriving.make.default false]
    transparent: bool,

    [@deriving.make.default Sparkline.make()]
    sparkline: Sparkline.t,

    [@deriving.make.default Gauge.make()]
    gauge: Gauge.t,

    targets: list(Target.t)
  };
};



module Panel = {
  type t =
    | Row(Row.t)
    | Graph(Graph.t)
    | Singlestat(Singlestat.t)
    ;

  let row = x => Row(x)
  let graph = x => Graph(x)
  let singlestat = x => Singlestat(x)

  let to_yojson = (panel) =>
    switch(panel) {
    | Row(row) => Row.to_yojson(row)
    | Graph(graph) => Graph.to_yojson(graph)
    | Singlestat(x) => Singlestat.to_yojson(x)
    };
};


module Template = {
  module Option = {
    [@deriving (make, to_yojson { strict: false })]
    type t = {
      [@deriving.make.default []]
      tags: list(string),

      text: string,
      value: string,
      
      [@deriving.make.default false]
      selected: bool
    };
  };

  [@deriving (make, to_yojson { strict: false })]
  type t = {
    name: string,

    [@deriving.make.default None]
    datasource: option(string),
    query: string,

    [@deriving.make.default None]
    label: option(string),

    [@deriving.yojson.key "allValue"]
    [@deriving.make.default None]
    all_value: option(string),

    [@deriving.yojson.key "tagValuesQuery"]
    [@deriving.make.default ""]
    tag_values_query: string,

    [@deriving.make.default None]
    current: option(Option.t),

    [@deriving.make.default []]
    options: list(Option.t),

    [@deriving.make.default 0]
    hide: int,

    [@deriving.make.default ""]
    regex: string,

    [@deriving.make.default 0]
    refresh: int,

    [@deriving.yojson.key "includeAll"]
    [@deriving.make.default false]
    include_all: bool,

    [@deriving.make.default false]
    multi: bool,

    [@deriving.make.default 0]
    sort: int,

    [@deriving.yojson.key "type"]
    [@deriving.make.default "query"]
    kind: string,
    
    [@deriving.make.default []]
    tags: list(string),

    [@deriving.make.default ""]
    tagsQuery: string,

    [@deriving.yojson.key "useTags"]
    [@deriving.make.default false]
    use_tags: bool
  };
};


module Dashboard = {
  [@deriving (make, to_yojson { strict: false })]
  type annotation =  {
    [@deriving.yojson.key "builtIn"]
    [@deriving.make.default 1]
    built_in: int,

    [@deriving.make.default "-- Grafana --"]
    datasource: string,

    [@deriving.yojson.key "enable"]
    [@deriving.make.default true]
    enable: bool,

    [@deriving.make.default true]
    hide: bool,

    [@deriving.yojson.key "iconColor"]
    [@deriving.make.default "rgba(0, 211, 255, 1)"]
    icon_color: string,

    [@deriving.make.default "Annotations & Alerts"]
    name: string,

    [@deriving.yojson.key "type"]
    [@deriving.make.default "dashboard"]
    kind: string,
  };
  let annotation = make_annotation;


  let default_refresh_intervals = [
      "5s",
      "10s",
      "30s",
      "1m",
      "5m",
      "15m",
      "30m",
      "1h",
      "2h",
      "1d"
    ];

  let default_time_options = [
      "5m",
      "15m",
      "1h",
      "6h",
      "12h",
      "24h",
      "2d",
      "7d",
      "30d"
    ];


  [@deriving (make, to_yojson { strict: false })]
  type timepicker = {
    [@deriving.make.default default_refresh_intervals]
    refresh_intervals: list(string),

    [@deriving.make.default default_time_options]
    time_options: list(string)
  };
  let timepicker = make_timepicker;


  [@deriving (make, to_yojson { strict: false })]
  type interval = {
    [@deriving.make.default "now-1h"]
    from: string,

    [@deriving.yojson.key "to"]
    [@deriving.make.default "now"]
    until: string
  };
  let interval = make_interval;

  [@deriving to_yojson { strict: false }]
  type templating = {
    list: list(Template.t)
  };


  [@deriving (make, to_yojson { strict: false })]
  type t = {
    [@deriving.make.default true]
    editable: bool,

    [@deriving.yojson.key "gnetId"]
    [@deriving.make.default None]
    gnet_id: option(int),

    [@deriving.yojson.key "graphTooltip"]
    [@deriving.make.default 0]
    graph_tooltip: int,

    [@deriving.make.default []]
    links: list(string),

    panels: list(Panel.t),

    [@deriving.yojson.key "schemaVersion"]
    [@deriving.make.default 16]
    schema_version: int,

    [@deriving.make.default "dark"]
    style: string,

    [@deriving.yojson.key "tags"]
    [@deriving.make.default []]
    tags: list(string),

    [@deriving.make.default interval()]
    time: interval,

    [@deriving.make.default timepicker()]
    timepicker: timepicker,

    [@deriving.yojson.key "timezone"]
    [@deriving.make.default ""]
    timezone : string,

    [@deriving.make.default None]
    templating: option(templating),

    title : string,

    [@deriving.make.default None]
    id: option(int),

    [@deriving.make.default None]
    uid : option(string),

    [@deriving.make.default 1]
    version : int
  }

  let make = (
    ~editable=?,
    ~gnet_id=?,
    ~graph_tooltip=?,
    ~links=?,
    ~panels=?,
    ~schema_version=?,
    ~style=?,
    ~tags=?,
    ~time=?,
    ~timepicker=?,
    ~timezone=?,
    ~templates=[],
    ~title,
    ~id=?,
    ~uid=?,
    ~version=?,
    ()
  ) =>
    make(
      ~editable?,
      ~gnet_id?,
      ~graph_tooltip?,
      ~links?,
      ~panels?,
      ~schema_version?,
      ~style?,
      ~tags?,
      ~time?,
      ~timepicker?,
      ~timezone?,
      ~templating=Some({list: templates}),
      ~title,
      ~id?,
      ~uid?,
      ~version?,
      ()
    );


};

type environment = {
  cluster: string,
  dashboard: Dashboard.t,
};

let size = (~width, ~height) => { w: width, h: height, x: -1, y : -1  };

let get_coordinates = (acc, position) => {
  if (position.w == 24 || acc.w == 24) {
    (0, acc.y + position.h)
  } else {
    if (acc.x == 0) {
      (acc.w, acc.y);
    } else {
      if (acc.x + acc.w >= 24) {
        (0, acc.y + position.h);
      } else {
        (acc.x + acc.w, acc.y);
      }
    }
  };
};

let layout = (dash) => {
  let f = ((acc, panels), item) =>
    switch (item) {
    | Panel.Graph(graph) => {
      let (x, y) = get_coordinates(acc, graph.position);
      let next_acc = { w: graph.position.w, h: graph.position.h, x, y };

      (next_acc, [Panel.Graph({...graph, position: next_acc}), ...panels]);
    }
    | Panel.Singlestat(singlestat) => {
      let (x, y) = get_coordinates(acc, singlestat.position);
      let next_acc = { w: singlestat.position.w, h: singlestat.position.h, x, y };

      (next_acc, [Panel.Singlestat({...singlestat, position: next_acc}), ...panels]);
    }
    | Panel.Row(row) => {
      let (x, y) = get_coordinates(acc, row.position);
      let next_acc = { w: row.position.w, h: row.position.h, x, y };

      (next_acc, [Panel.Row({...row, position: next_acc}), ...panels]);
    }
    };

    let dash = List.concat(dash);
    let (_, panels) = List.fold_left(f, ({ w: 0, h: 0, x: 0, y: 0 }, []), dash);
    List.rev(panels);
};

let _gen = (~namespace, ~environmets) => {
  prerr_endline("Generating Grafana:");

  environmets |> List.iter(({cluster, dashboard}) => {
    let path = Printf.sprintf("_gen/grafana/%s/%s", cluster, namespace);
    let dashboard_path = path ++ "/dashboard.json";
    ignore(Unix.system("mkdir -p " ++ path));
    ignore(Unix.system(Printf.sprintf("rm -r %s", dashboard_path)));

    Printf.eprintf("-> %s\n", dashboard_path);

    let out = open_out(dashboard_path);
    let str =
      dashboard
      |> Dashboard.to_yojson
      |> Yojson.Safe.pretty_to_string;

    output_string(out, str ++ "\n");
    close_out(out);
  });
};
