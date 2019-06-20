# Refana

Define Grafana dashboards with ReasonML.

- [Homepage](https://rizo.github.io/refana/refana)
- [API reference](https://rizo.github.io/refana/refana/Refana)


## Usage

### Defining metrics

```reason
let cpu = (~name) =>
  Grafana.(Query.prometheus(Prometheus_query {
    "id": "CPU",
    "expr": fmt("rate(cpu_seconds{container_name=~'%s'}[30s])", name),
    "format": "time_series",
    "legend": "cpu_seconds@{{pod_name}}"
  }));

let mem = (~name) =>
  Grafana.(Query.prometheus(Prometheus_query {
    "id": "MEM",
    "expr": fmt("sum(memory_rss{container_name=~'%s'}) by (pod_name)", name),
    "format": "time_series",
    "legend": "memory_rss@{{pod_name}}"
  }));
```

### Creating panels

```
let name = "my-app";

let metrics = Grafana.[
  Panel.row(Row {
    "title": "My metrics",
    "position": size(~width=24, ~height=1),
  }),

  Panel.graph(Graph {
    "title": "Container CPU usage",
    "position": size(~width=12, ~height=9),
    "queries": [cpu(~name)]
  }),
  Panel.graph(Graph {
    "title": "Container memory usage",
    "position": size(~width=12, ~height=9),
    "queries": [Std.Grafana.Metrics.mem(~name)]
  })
];
```

### Creating dashboards

```
let dash = Grafana.Dashboard {
  "title": "My Monitoring Dashboard",
  "panels": Grafana.panels([metrics])
}
```
