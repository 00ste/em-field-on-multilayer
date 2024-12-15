# Set the terminal type to postscript and specify the output file
set terminal postscript eps enhanced color
set output 'plot-2b.eps'

# Set labels for the axes
set xlabel 'z [m]'
set ylabel 'Re(E_x), Im(E_x), Re(E_z), Im(E_z) [V/m]'

# Set title for the plot
set title 'Results of second functional test'

# Set grid for better readability
set grid

# Plot the data from the CSV file
# The using statement specifies which columns to use
plot 'output-2b.csv' using 2:3 with lines title 'Re(E_x)', \
     'output-2b.csv' using 2:4 with lines title 'Im(E_x)', \
     'output-2b.csv' using 2:7 with lines title 'Re(E_z)', \
     'output-2b.csv' using 2:8 with lines title 'Im(E_z)', \
