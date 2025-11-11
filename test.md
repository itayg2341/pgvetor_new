# How to Test and Compile the New C++ Code

## Compiling the Code

The project now includes a mix of C and C++ code. The build system has been updated to handle this automatically. To compile the code, simply run the `make` command from the root of the project directory:

```bash
make
```

This will compile both the C and C++ source files and link them together into a single shared library.

## Running the Tests

The project includes a suite of regression tests that verify the functionality of the code. To run the tests, you will need to have a PostgreSQL server running. The following steps will guide you through the process:

1.  **Install PostgreSQL:** If you do not have PostgreSQL installed, you will need to install it. The following command will install the necessary packages on a Debian-based system:

    ```bash
    sudo apt-get install -y postgresql-17 postgresql-server-dev-17
    ```

2.  **Start the PostgreSQL Server:** Once PostgreSQL is installed, you will need to start the server. The following command will start the server:

    ```bash
    sudo service postgresql start
    ```

3.  **Create a Test User:** The regression tests run as the "root" user. You will need to create a new user in PostgreSQL with this name and grant it superuser privileges. The following command will create the user:

    ```bash
    sudo -u postgres createuser -s root
    ```

4.  **Run the Tests:** Once the server is running and the test user has been created, you can run the regression tests. The following command will run the tests:

    ```bash
    make installcheck
    ```

This will compile the code, install the extension, and run the regression tests. All tests should pass.
