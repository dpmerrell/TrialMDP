
#' Connect to a trial design SQLite database
#'
#' @param sqlite_filename path to trial design SQLite database
#'
#' @return an RSQLite database connection object
connect_to_results <- function(sqlite_filename){
  conn = RSQLite::dbConnect(RSQLite::SQLite(), sqlite_filename, flags=RSQLite::SQLITE_RO);
  return(conn);
}


#' Get information from a trial design SQLite database
#'
#' Given your current contingency table, retrieve the size and treatment allocation of the next trial stage.
#' 
#' @param db_conn RSQLite database connection object (connection to the trial design database)
#' @param a0 entry A0 of your current contingency table
#' @param a1 entry A1 of your current contingency table
#' @param b0 entry B0 of your current contingency table
#' @param b1 entry B1 of your current contingency table
#' 
#' @return a list of values, including the size of the next stage ("BlockSize") and number of patients to allocate to A ("AAllocation") 
fetch_result <- function(db_conn, a0, a1, b0, b1){
  sql_str = paste("SELECT * FROM RESULTS WHERE (A0, A1, B0, B1) = (",
                  a0, ",", a1, ",", b0, ",", b1, ")");
  result = DBI::dbGetQuery(conn, sql_str);
  return(result);
}
