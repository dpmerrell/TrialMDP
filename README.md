<!-- badges -->
<!-- GitHub actions -->
![build and test](https://github.com/dpmerrell/blockRARopt/workflows/build_and_test/badge.svg)


# TrialMDP 

An optimization solver that designs blocked Response-Adaptive Randomized (RAR) trials.

An R package, implemented in C++.

See the TrialMDP manuscript for more information:

> [*A Markdov Decision Process for Response-Adaptive Randomization in Clinical Trials*. David Merrell, Thevaa Chandereng, and Yeonhee Park. Computational Statistics and Data Analysis. 2023 (to appear)](https://arxiv.org/abs/2109.14642)

See the [TrialMDP-analyses repository](https://github.com/dpmerrell/TrialMDP-analyses) for code that reproduces all of the results in the manuscript.

## R package dependencies

* Rcpp (>= 1.0.5)
* RSQLite
* BH

See the `DESCRIPTION` file for other details. 

## Installation
_Note: We only support Mac and Linux currently._

Follow these steps:

* Install the SQLite3 dev libraries. E.g., for Ubuntu:
```
$ sudo apt install libsqlite3-dev
```
The `TrialMDP` package writes its output to a SQLite database.

* Install the `TrialMDP` package. We aren't on CRAN yet&mdash;for now we recommend using `devtools`:
```R
> library("devtools")
Loading required package: usethis
> devtools::install_github("https://github.com/dpmerrell/TrialMDP")
```

## Basic usage

### Generate an optimal trial design and save to a SQLite database
```R
> # Load the library
> library("TrialMDP")
> # Design a trial for 44 patients; failure cost=4, stage cost=0.025.
> TrialMDP::trial_mdp(44, 4.0, 0.025, "results.sqlite", min_size=8, block_incr=2)
Solver initialized.
	N patients: 44
	Min block size: 8
	Block increment: 2
	Allocations: {0.2, ..., 0.8} (7)
	Failure cost: 4
	Block cost: 0.025
	Test statistic: scaled_cmh
Solving.
Action:
	Block size: 8
	N_A: 4
	N_B: 4
Expected values:
	Failure: -0.151702
	RemainingBlocks: 3.072850
	ScaledCMH: 1.039444
	TotalReward: 1.569432
Solver completed.
Saved to file: results.sqlite
```

### Use the saved trial design
```R
> # Connect to the trial design SQLite database
> conn = TrialMDP::connect_to_results("results.sqlite")
> # Get the action for the *empty* contingency table, i.e., (0,0,0,0)
> res = TrialMDP::fetch_result(conn, 0,0,0,0)
> print(res)
  A0 A1 B0 B1 BlockSize AAllocation   Failure RemainingBlocks ScaledCMH
1  0  0  0  0         8           4 -0.151702         3.07285  1.039444
  TotalReward
1    1.569432
```


## Licensing

We distribute the contents of this repository under an MIT license. See LICENSE.txt for details.
