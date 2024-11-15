# Project 4: Packet Trace Analyzer
---
A `Trace Packet` is a data file that captures network traffic by recording each packet transmitted across a data link.

## Running theh Packet Trace Analyzer

`proj4` must be run like this

```bash
./proj4 -r trace_file -i|-s|-t|-m
```
- `-r` is `required` and specifies the `name of the packet`
- `One` and `only One` Mode must be specified
    - `-i` is `trace info mode`
    - `-s` is `size analysis mode`
    - `-t` is `tcp packet tracing mode `
    - `-m` is `traffic matrix mode`

## Samples Management

```bash
chmod +x extract_samples.sh cleanup_samples.sh run_project.sh
```
```bash
./extract_samples.sh
```
```bash
./cleanup_samples.sh
```

```bash
dos2unix extract_samples.sh cleanup_samples.sh
```

```bash
./run_project.sh 100-pkts.trace -i
```