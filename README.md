# em-fields-on-multilayer

Tool that calculates the values of the electromagnetic field which is obliquely incident on a multilayer.

# Dependencies

- g++
- make

# Building

Simply run `make` inside the root directory of the project to build the full tool. The tests inside the `tests` directory can be built in the same way.

# Usage

```bash
./fields path/to/input-data-file.txt path/to/input-points-file.txt path/to/output-file.txt
```

Where:
- `path/to/input-data-file.txt` is the path to the file that contains data about the experiment to be simulated.
- `path/to/input-points-file.txt` is the path to the file that contains the list of points where the fields should be calculated.
- `path/to/output-file.txt` is the path to the file that will store the results of the simulated experiment. The file will be overwritten.

> Examples of both input files can be found inside the `tests/test_data` directory.

## Input data file format

The input data file should be in the following format:

```
z_0, z_1, ..., z_{N-1}
epsilon_0_real, epsilon_0_imag, mu_0_real, mu_0_imag
epsilon_0_real, epsilon_0_imag, mu_0_real, mu_0_imag
...
epsilon_0_real, epsilon_0_imag, mu_0_real, mu_{N-1}_imag
epsilon_N_real, epsilon_N_imag, mu_N_real, mu_N_imag
incident_field_amplitude | incident_field_amplitude_real, incident_field_amplitude_imag
angle_of_incidence
wavelength
TM | TE
```
Where:
- the first line contains the z coordinates of the interfaces.
- assuming N-1 interfaces specified in the first line, the following N lines contain the real and imaginary parts of the **relative** constitutive parameters $\varepsilon_r$ and $\mu_r$ of each medium.
- The next line contains the magnitude of the vector phasor of the incident electric field, as such, it can be an imaginary number. If it is only real, the imaginary part can be omitted.
- The next line contains the angle of incidence of the field with respects to the first interface.
- The next line contains the wavelength in $\mathrm{nm}$.
- The last line contains the polarization of the wave, either the literal `TM` or `TE`. If another string is specified, TE polarization is assumed by default, and a warning message will be displayed in the stdout.

## Input points file format

```
xp1, zp1
xp1, zp1
...
xpn, zpn
```
The input points file is a list of pairs `xp, zp`, which are both real floating point numbers.