library(RSQLite)

# Step 1: Create a connection to a SQLite database
con <- dbConnect(SQLite(), "test_database.db")  # Creates 'test_database.db' in the current directory

# Step 2: Write a sample table to the database
dbWriteTable(con, "example_table", data.frame(id = 1:5, value = letters[1:5]))

# Step 3: Read the table back
result <- dbReadTable(con, "example_table")
print(result)

# Step 4: Disconnect from the database
dbDisconnect(con)
