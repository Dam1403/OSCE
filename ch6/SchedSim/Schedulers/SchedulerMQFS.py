from process import Process


class SchedulerMQFS:
    """
    Template for Schedulers
    """
    def __init__(self, time_quantum=40000, levels=5, quant_mult=4):
        self.name = "Multi Queue Feedback Scheduler"
        self.levels = levels
        self.ready_queue: list[list[Process]] = [[] for level in range(0,levels)]
        self.quantums: list[int] = [level * quant_mult for level in range(0,levels)]
        self.blocked_queue: list[Process] = []

        self.curr_process: Process = None
        self.time_quantum = time_quantum

    def __str__(self):
        result = ""

        for level in range(0, self.levels):
            result += f"Level {level}:["

            for process in self.ready_queue[level]:
                result += f" {process.id}, "
            result += "\n"
        return result

    def remove_from_ready(self, process):
        for level in range(0, self.levels):
            try:
                self.ready_queue[level].remove(process)
            except ValueError:
                continue

    """
    Return the Process object that should run
    Return None if no process should run
    """
    def get_curr_process(self):
        #print(self)
        return self.curr_process

    def new_process_arrival(self, process):
        self.ready_queue[0].append(process)
        self.get_next_process()

    def quantum_exhausted(self, process):
        for level in range(0, self.levels - 1):
            try:
                self.ready_queue[level].remove(process)
                self.ready_queue[level + 1].append(process)
            except ValueError:
                continue
        self.get_next_process()

    def process_exit(self, process):
        self.remove_from_ready(process)
        self.get_next_process()

    def io_block(self, process: "Process" = None):
        if process is None:
            if self.curr_process is None:
                return
            self.curr_process.state = Process.BLOCKED
            self.blocked_queue.append(self.curr_process)
            self.get_next_process()
            return

        for level in range(0, self.levels):
            try:
                index = self.ready_queue[level].index(process)
                self.ready_queue[level][index].state = Process.BLOCKED
            except ValueError:
                continue

    def io_unblock(self, process: "Process" = None):
        if process is None:
            if self.curr_process is None:
                return
            self.curr_process.state = Process.READY
            self.ready_queue[0].append(self.blocked_queue.pop(0))
            self.get_next_process()
            return

        for level in range(0, self.levels):
            try:
                index = self.ready_queue[level].index(process)
                self.ready_queue[level][index].state = Process.READY
            except ValueError:
                continue

    def get_next_process(self):
        for level in range(0, self.levels):
            for process in self.ready_queue[level]:
                if process.state == Process.READY \
                        or process.state == Process.RUNNING:
                    self.curr_process = process
                    return
        self.curr_process = None
