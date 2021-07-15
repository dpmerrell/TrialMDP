<!-- badges -->
<!-- GitHub actions -->
![build and test](https://github.com/dpmerrell/blockRARopt/workflows/build_and_test/badge.svg)


# TrialMDP 

An optimization solver that designs adaptive multistage clinical trials.

An R package, implemented in C++.

[See the TrialMDP manuscript for more information.](https://docs.google.com/presentation/d/15oAJu6PEweWosXLDnaXX5xcuBJHCEPyHMZB1zDjJXd0/edit?usp=sharing)
TODO add manuscript link

See the [TrialMDP-analyses repository](https://github.com/dpmerrell/TrialMDP-analyses) for code that reproduces all of our performance evaluations.

## Installation
_Note: We only support Linux and Mac currently._

Follow these steps:

* Install the SQLite3 dev libraries. E.g., for Ubuntu:
```
$ sudo apt install sqlite3-dev
```
The `TrialMDP` package uses SQLite to write its output to a database.

* Install the `TrialMDP` package. We aren't on CRAN yet&mdash;for now we recommend using `devtools`:
```R
> library("devtools")
Loading required package: usethis
> devtools::install_github("https://github.com/dpmerrell/TrialMDP")
```

## Basic usage
```R
> # Load the library
> library("TrialMDP")
> # Design a trial for 44 patients; failure cost=4, stage cost=0.025.
> # Save to a sqlite file.
> TrialMDP::trial_mdp(44, 4.0, 0.025, "results.sqlite", min_size=8, block_incr=2)
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

## Licensing

We distribute TrialMDP under an MIT license. See LICENSE.txt for details.
