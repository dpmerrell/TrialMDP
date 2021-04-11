library("blockRARopt")

#blockRARopt::block_rar_opt(48, 2, 0.0, 0.0, "results.sqlite", 
#                           #prior_a0=0.5, prior_a1=0.5, prior_b0=0.5, prior_b1=0.5,
#                           prior_a0=1, prior_a1=1, prior_b0=1, prior_b1=1,
#                           test_statistic="harmonic_mean", act_l=0.2, act_u=0.8, act_n=7) 

blockRARopt::block_rar_opt(40, 2, 0.5, 0.001, "results.sqlite", 
                           #prior_a0=0.5, prior_a1=0.5, prior_b0=0.5, prior_b1=0.5,
                           prior_a0=0.5, prior_a1=0.5, prior_b0=0.5, prior_b1=0.5,
                           test_statistic="scaled_cmh", act_l=0.2, act_u=0.8, act_n=7) 

print("About to connect")
conn = blockRARopt::connect_to_results("results.sqlite")

print("Successfully connected; about to fetch")
res = blockRARopt::fetch_result(conn, 0,0,0,0)

print("successfully fetched")
print(res)
