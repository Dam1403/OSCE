from process import Process


class IScheduler:
    """
    Template for Schedulers
    """
    def __init__(self):
        pass

    """
    Return the Process object that should run
    Return None if no process should run
    """
    def get_curr_process(self):
        print(f"{self.name} get_curr_process not overridden")

    def new_process_arrival(self, process):
        print(f"{self.name} new_process_arrival not overridden")

    def quantum_exhausted(self, process):
        print(f"{self.name} quantum_exhausted not overridden")

    def process_exit(self,process):
        print(f"{self.name} process_exit not overridden")

    def io_block(self, process: "Process" = None):
        print(f"{self.name} io_block not overridden")

    def io_unblock(self, process: "Process" = None):
        print(f"{self.name} io_unblock not overridden")
