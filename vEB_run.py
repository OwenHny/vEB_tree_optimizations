#This file was written with the help of chatGPT
import subprocess

# Initialize an empty list to store the results
results = []

# Iterate through all possible combinations of inputs
for num1 in range(7, 14):
    for num2 in range(2,10):
        #for boolean_input in range(2):
            # Convert the integer inputs to string arguments
            args = [str(num1), str(num2), str(0)]

            # Start the C program with the generated arguments
            process = subprocess.Popen(['./vEB'] + args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            # Wait for the program to finish
            stdout, stderr = process.communicate()

            # Parse the output of the program and store the results
            output = stdout.decode().strip()
            result = [float(x) for x in output.split()]
            results.append(result)

            # Print the inputs and output of the program
            print(f"{args}, {result}")

