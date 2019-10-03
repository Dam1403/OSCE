from typing import Callable


class Process:

    _id = 0
    READY = 1
    BLOCKED = 2
    RUNNING = 3
    DEAD = 4

    def __init__(self, burst_time, arrival_time, priority=0):
        Process._id += 1
        self.id = Process._id
        self.state = Process.READY
        self.burst_time = burst_time
        self.burst_time_remaining = burst_time
        self.io_block_dict = {}
        self.arrival_time = arrival_time
        self.priority = priority

    def __eq__(self, other):
        return other.id == self.id

    def __str__(self):
        the_str = f"PID: {str(self.id).zfill(6)} Arrival_Time: {str(self.arrival_time).zfill(8)}" +\
                  f" Burst Time: {str(self.burst_time_remaining).zfill(8)}"
        return the_str







