
connect_to_results <- function(sqlite_filename){
  conn = RSQLite::dbConnect(RSQLite::SQLite(), sqlite_filename, flags=RSQLite::SQLITE_RO);
  return(conn);
}


fetch_result <- function(db_conn, a0, a1, b0, b1){
  sql_str = paste("SELECT * FROM RESULTS WHERE (A0, A1, B0, B1) = (",
                  a0, ",", a1, ",", b0, ",", b1, ")");
  result = DBI::dbGetQuery(conn, sql_str);
  return(result);
}
