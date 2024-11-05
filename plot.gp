# Set the terminal type to postscript and specify the output file
set terminal postscript eps enhanced color
set output 'plot.eps'

# Set labels for the axes
set xlabel 'z [m]'
set ylabel 'Re(E_x), Im(E_x) [V/m]'

# Set title for the plot
set title 'Results of first functional test'

# Set grid for better readability
set grid

# Plot the data from the CSV file
# The using statement specifies which columns to use
plot 'output-first-test-real-imag.csv' using 2:3 with lines title 'Real part', \
     'output-first-test-real-imag.csv' using 2:4 with lines title 'Imaginary part'
