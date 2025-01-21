
# Define a simple function to square a number
square <- function(x) {
  return(x * x)
}

# Create a list of numbers
numbers <- list(1, 2, 3, 4, 5)

# Apply the square function to each element of the list
squared_numbers <- lapply(numbers, square)

print(squared_numbers)


#2. LOOPING 

# Create a list of numbers
numbers <- list(1, 2, 3, 4, 5)

# Initialize an empty list to store squared numbers
squared_numbers <- list()

# Use a for loop to square each number
for (i in 1:length(numbers)) {
  squared_numbers[[i]] <- numbers[[i]] * numbers[[i]]
}

print(squared_numbers)

#3.
# Define a simple S3 class for a point
point <- function(x, y) {
  structure(list(x = x, y = y), class = "point")
}

# Define a method to print a point object
print.point <- function(p) {
  cat("Point(", p$x, ", ", p$y, ")\n", sep = "")
}

# Create a point object
p <- point(2, 3)

# Print the point object
print(p)