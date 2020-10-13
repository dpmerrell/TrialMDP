library("RSQLite")

connect_to_results <- function(sqlite_filename){
  conn = dbConnect(SQLite(), sqlite_filename, flags=SQLITE_RO);
  conn;
}


fetch_result <- function(db_conn, a0, a1, b0, b1){
  sql_str = paste("SELECT * FROM Results WHERE (A0, A1, B0, B1) = (",
                  a0, a1, b0, b1, ")");
  result = dbGetQuery(conn, sql_str);
  result;
}
