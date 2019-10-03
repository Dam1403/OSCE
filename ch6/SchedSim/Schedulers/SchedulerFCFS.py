from sys import maxsize
import Schedulers.IScheduler as IScheduler
from process import Process


class SchedulerFCFS(IScheduler.IScheduler):

    def __init__(self, time_quantum=maxsize):
        self.name = "Non Preemptive FCFS"
        self.time_quantum = time_quantum
        self.ready_queue: list[Process] = []
        self.blocked_queue: list[Process] = []
        self.curr_process = None

    def get_curr_process(self):
        return self.curr_process

    def new_process_arrival(self, process):
        self.ready_queue.append(process)
        self.calc_next_process()

    def quantum_exhausted(self, process):
        pass

    def process_exit(self, process):
        self.ready_queue.remove(process)
        self.calc_next_process()

    def io_block(self, process: Process = None):
        if process is None:
            self.curr_process.state = Process.BLOCKED
            self.blocked_queue.append(self.curr_process)

        elif self.curr_process is not None:
            process.state = Process.BLOCKED
        self.calc_next_process()

    def io_unblock(self, process: Process = None):
        if process is None:
            self.blocked_queue.pop(0).state = Process.READY
        else:
            process.state = Process.READY

    def calc_next_process(self):
        self.curr_process = None
        for process in self.ready_queue:
            if process.state == Process.READY or process.state == Process.RUNNING:
                self.curr_process = process
                print(f"P{self.curr_process.id}: Should Execute")
                break
