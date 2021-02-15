library("blockRARopt")

blockRARopt::block_rar_opt(64, 16, 2.0, 0.5, "results.sqlite", 
                           prior_a0=0.5, prior_a1=0.5, prior_b0=0.5, prior_b1=0.5,
                           test_statistic="cmh") 

print("About to connect")
conn = blockRARopt::connect_to_results("results.sqlite")

print("Successfully connected; about to fetch")
res = blockRARopt::fetch_result(conn, 0,0,0,0)

print("successfully fetched")
print(res)
