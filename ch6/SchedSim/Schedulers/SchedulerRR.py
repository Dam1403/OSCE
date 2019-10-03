from process import Process
import Schedulers.IScheduler as IScheduler


class SchedulerRR(IScheduler.IScheduler):
    """
    Template for Schedulers
    """

    class RRProcess:
        def __init__(self, process):
            self.process = process
            self.next: "RRProcess" = None

    def __init__(self, time_quantum=30000):
        self.name = "Round Robin"
        self.time_quantum = time_quantum

        self.blocked_queue: list["SJFProcess"] = []
        self.curr_process: "Process" = None
        self.curr_rr_process: "SchedulerRR.RRProcess" = None
        self.curr_index = 0

    """
    Return the Process object that should run
    Return None if no process should run
    """
    def get_curr_process(self):
        return self.curr_process

    def new_process_arrival(self, process):
        new_rr_process = self.RRProcess(process)

        if self.curr_rr_process is None:
            self.curr_rr_process = new_rr_process
            new_rr_process.next = new_rr_process
        else:
            new_rr_process.next = self.curr_rr_process.next
            self.curr_rr_process.next = new_rr_process

    def quantum_exhausted(self, process):
        self.curr_rr_process = self.curr_rr_process.next
        self.curr_process = self.curr_rr_process.process

    def process_exit(self, process):
        process.state = Process.DEAD
        self.get_next_process()

    def io_block(self, process: "Process" = None):
        if process is None:
            self.blocked_queue.append(self.curr_rr_process)
            self.curr_rr_process.process.state = Process.BLOCKED
        else:
            process.state = Process.BLOCKED
        self.get_next_process()

    def io_unblock(self, process: "Process" = None):
        if process is None:
            self.blocked_queue.pop(0)
            self.curr_rr_process.state = Process.READY
        else:
            process.state = Process.READY
        self.get_next_process()

    def get_next_process(self):
        if self.curr_rr_process == self.curr_rr_process.next:
            if self.curr_rr_process.process.state == Process.DEAD:
                self.curr_rr_process = None
                self.curr_process = None

            elif self.curr_rr_process.process.state == Process.BLOCKED:
                self.curr_process = None
            return

        loop_check = self.curr_rr_process
        next_proc = self.curr_rr_process.next
        while next_proc is not loop_check:

            if next_proc.process.state == Process.DEAD:
                self.curr_rr_process.next = next_proc.next
                next_proc = next_proc.next

            elif next_proc.process.state == Process.BLOCKED:
                self.curr_rr_process = next_proc
                next_proc = self.curr_rr_process.next
            else:
                self.curr_rr_process = next_proc
                break

        self.curr_process = self.curr_rr_process.process








"""
add 
if curr_proc is None 

"""