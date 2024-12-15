# Set the terminal type to postscript and specify the output file
set terminal postscript eps enhanced color
set output 'plot-2.eps'

# Set labels for the axes
set xlabel 'z [m]'
set ylabel '|E_x|, |E_x|, |E_z| [V/m]'

# Set title for the plot
set title 'Results of second functional test'

# Set grid for better readability
set grid

# Plot the data from the CSV file
# The using statement specifies which columns to use
plot 'output-2.csv' using 2:3 with lines title '|E_x|', \
     'output-2.csv' using 2:5 with lines title '|E_y|', \
     'output-2.csv' using 2:7 with lines title '|E_z|', \
