## Assignment 3

This project simulates a concurrent stock inventory management system. It processes 'in' and 'out' transactions using multiple pthreads, managing shared stock levels and transaction states via global arrays. Mutexes ensure thread-safe inventory access. Transactions dynamically progress, with 'out' orders waiting for sufficient stock. The system outputs transaction outcomes and final inventory status.
