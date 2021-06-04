library("blockRARopt")

blockRARopt::block_rar_opt(44, 3.0, 0.1, "results.sqlite",
                           min_size=6, block_incr=1, 
                           prior_a0=1.0, prior_a1=1.0, prior_b0=1.0, prior_b1=1.0,
                           test_statistic="hm_dsq", act_l=0.2, act_u=0.8, act_n=7) 
#blockRARopt::block_rar_opt(44, 0.3, 0.01, "results.sqlite",
#                           min_size=1, block_incr=2, 
#                           prior_a0=1.0, prior_a1=1.0, prior_b0=1.0, prior_b1=1.0,
#                           test_statistic="scaled_cmh_2o", act_l=0.2, act_u=0.8, act_n=7) 

print("About to connect")
conn = blockRARopt::connect_to_results("results.sqlite")

print("Successfully connected; about to fetch")
res = blockRARopt::fetch_result(conn, 0,0,0,0)

print("successfully fetched")
print(res)
