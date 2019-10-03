from process import Process
import Schedulers.IScheduler as IScheduler
import heapq
import sys


class SchedulerSJF(IScheduler.IScheduler):
    """
    Template for Schedulers
    """

    class SJFProcess:
        def __init__(self, process: Process):
            self.process = process

        def __lt__(self, other):
            if self.process.burst_time == other.process.burst_time:
                return self.process.arrival_time < self.process.arrival_time
            return self.process.burst_time_remaining < other.process.burst_time_remaining

    def __init__(self, time_quantum=sys.maxsize):
        self.name = "Preemptive SJF"
        self.time_quantum = time_quantum
        self.ready_heap: list["SJFProcess"] = []
        self.blocked_dict: dict[int, "SJFProcess"] = dict()
        self.blocked_queue: list["SJFProcess"] = []
        self.curr_process = None


    """
    Return the Process object that should run
    Return None if no process should run
    """
    def get_curr_process(self):
        return self.curr_process

    def new_process_arrival(self, process):
        heapq.heappush(self.ready_heap, self.SJFProcess(process))

    def quantum_exhausted(self, process):
        pass

    def process_exit(self, process):
        heapq.heappop(self.ready_heap)
        self.calc_next_process()

    def io_block(self, process: Process = None):
        sjf_process: SchedulerSJF.SJFProcess = heapq.heappop(self.ready_heap)
        sjf_process.process.state = Process.BLOCKED

        if process is None:
            self.blocked_queue.append(sjf_process)
        else:
            self.blocked_dict[sjf_process.process.id] = sjf_process

        self.calc_next_process()

    def io_unblock(self, process: Process = None):
        if process is None:
            sjf_process: SchedulerSJF.SJFProcess = self.blocked_queue.pop(0)

        else:
            sjf_process: SchedulerSJF.SJFProcess = self.blocked_dict[process.id]

        sjf_process.process.state = Process.READY
        heapq.heappush(self.ready_heap, sjf_process)
        self.calc_next_process()

    def calc_next_process(self):
        if len(self.ready_heap) == 0:
            self.curr_process = None
            print(f"IDLE")
        else:
            self.curr_process = self.ready_heap[0].process
            self.curr_process.state = Process.RUNNING
            print(f"P{self.curr_process.id}: Should Execute")

