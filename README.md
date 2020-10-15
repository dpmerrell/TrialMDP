# blockRARopt

An optimization solver for block RAR RCT design -- all wrapped up in an R package.

[See these Google slides for more information.](https://docs.google.com/presentation/d/15oAJu6PEweWosXLDnaXX5xcuBJHCEPyHMZB1zDjJXd0/edit?usp=sharing)

## Installation
_Note: So far I've only tested this on Ubuntu. We hope to support Mac and Windows in the future._

Follow these steps:

* Install the SQLite3 dev libraries. E.g., for Ubuntu:
```
$ sudo apt install sqlite3-dev
```
The `blockRARopt` package uses SQLite to write its output to a database.

* Install the `blockRARopt` package. We aren't on CRAN yet -- for now we recommend using `devtools`:
```R
> library("devtools")
Loading required package: usethis
> devtools::install_github("https://github.com/dpmerrell/blockRARopt")
```

## Basic usage
```R
> library("blockRARopt")
> blockRARopt::block_rar_opt(10, 1, 0.0, 100.0, "results.sqlite")
about to initialize solver
Initialized solver; about to solve
FIRST MOVE:
	block size: 10
		N_A: 5
		N_B: 5

EXPECTED REWARD:
	Wald statistic: 0.533882
	Failures: 0.878906
	Number of blocks: 1

	Total: -99.4661
solver completed
Saved to file: results.sqlite
```