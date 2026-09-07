# Project overview
this is a hardware-in-the-loop-style aircraft pitch control split across two microcontrollers, with the results streamed to a PC and visualized in FlightGear.

## Architecture

ESP32 = the "plant." It runs a discrete-time simulation of an aircraft's longitudinal (pitch) dynamics: a 3-state linear state-space model ẋ = Ax + Bu (pitch rate, angle of attack, pitch angle), integrated with Euler's method at a 5 ms step for 3,000 steps. Each step it prints the pitch angle θ over serial and reads back the elevator command.
Arduino Uno = the controller. It reads θ, computes an error against a setpoint, and sends back an elevator deflection δ, saturated to ±1.57 rad. The two boards talk to each other and to the PC over UART at 9600 baud.
PC (Python, run_sim.py). Reads both serial ports until the ESP32 sends STOP, plots θ and δ vs. time, then propagates a simple kinematic flight path (constant 125 m/s, fixed heading) from 2 km downrange / 500 m up toward runway 33R at Toronto Pearson, converts NED→lat/lon/alt, and feeds it to FlightGear via UDP using flightgear_python (FGConnector.py).
PlatformIO handles the two build targets (esp32dev and uno) with source filters so each board compiles only its own main_*.cpp.

## Two examples

PID — a hand-written PID class (Kp=6, Ki=1, Kd=3) whose setpoint is the glideslope angle atan(height/downrange), so the aircraft pitches to a landing approach.
NeuralNetwork — the PID is replaced with a small neural network. NN_training/train_the_NN.py uses PyTorch to train a 2→5→5→1 ReLU MLP on data.csv (200 rows of error → control signal) to imitate a controller, then tests it against the same A/B model in Python with a ramping setpoint (0.5→1.25 rad). The learned weights and biases are hard-coded into controller.cpp, and neuralMath.h provides template matrix-multiply, add, and ReLU functions so the Uno performs the forward pass in plain C++ with no ML library.

## Requirements: two UART microcontrollers (Arduino/ESP32), PlatformIO, PyTorch, FlightGear and pyserial/flightgear_python on the PC side.
