# Linux Kernel Module – Process Info Filter

This project was a coursework assignment from my Operating Systems class to demonstrate basic Linux kerel module programming.  It accepts a PID threshold as a module parameter and logs information about all processes with a higher PID. Output is printed using `dmesg`.

To build and run:
```bash
make                                              # Compile the kernel module
sudo insmod process_info_lkm.ko inp_pid=1000      # Insert the module with PID threshold
dmesg | tail                                      # View the kernel log output
sudo rmmod process_info_lkm                       # Remove the module
```


## Technologies Used

- **C** – Low-level programming
- **Linux Kernel API** – Process traversal and module interfaces
- **Makefile** – Compiles module using `make`
