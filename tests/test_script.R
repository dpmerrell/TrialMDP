library("TrialMDP")

TrialMDP::trial_mdp(44, 4.0, 0.025, "results.sqlite",
                    min_size=8, block_incr=2, 
                    prior_a0=1.0, prior_a1=1.0, prior_b0=1.0, prior_b1=1.0,
                    test_statistic="scaled_cmh", act_l=0.2, act_u=0.8, act_n=7) 

print("About to connect")
conn = TrialMDP::connect_to_results("results.sqlite")

print("Successfully connected; about to fetch")
res = TrialMDP::fetch_result(conn, 0,0,0,0)

print("successfully fetched")
print(res)
