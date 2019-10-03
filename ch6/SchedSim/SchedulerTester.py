
import random
from Schedulers import *

from config import CONFIG
from process import Process
from events import CpuEvent, EVENT_MANAGER
import Units

"""
Doug's useless Process Scheduler tester. 
There might be docs if I keep working on this probably not though. 
"""


class InterruptJob:
    def __init__(self, handler, args, latency):
        self.handler = handler
        self.args = args
        self.latency = latency


class CpuState:
    def __init__(self,scheduler=None):
        self.state = "WAITING"
        self.current_job = None
        self.current_quantum = 0
        self.interrupt_queue = []
        self.work_counter = 0
        self.idle_ticks = 0
        self.last_exit_tick = 0
        self.wtt_rolling_time_sum = 0
        self.tut_rolling_time_sum = 0

    def add_interrupt(self, handler, args, latency=CONFIG.interrupt_latency_micsec):
        interrupt_job = InterruptJob(handler, args, latency)
        self.interrupt_queue.append(interrupt_job)


def gen_arrivals(num=0):
    if num == 0:
        num = CONFIG.total_jobs

    for i in range(0, num):
        max_burst = CONFIG.runtime_microseconds * CONFIG.job_to_runtime_ratio
        burst = random.randrange(100, max_burst)
        arrival_time = random.randrange(0, CONFIG.runtime_microseconds)

        process = Process(burst, arrival_time)
        event = CpuEvent(CpuEvent.NEW_PROCESS, process)

        EVENT_MANAGER.add_event(event, arrival_time)

        print(f"Added:  {str(process)}")
    print("\n=============== EXECUTION ==============\n")


def gen_io_events(block_per_sec=CONFIG.avg_io_block_per_sec):
    total = CONFIG.runtime_microseconds * 50

    time_sum = chunk = Units.S / block_per_sec
    print(time_sum)
    while time_sum < total:

        event = CpuEvent(CpuEvent.IO_BLOCK, None)
        EVENT_MANAGER.add_event(event, time_sum)

        event = CpuEvent(CpuEvent.IO_UNBLOCK, None)
        EVENT_MANAGER.add_event(event, time_sum + CONFIG.io_block_time_micsec)

        time_sum += chunk


def gen_io_blocks_process(act_burst, max_burst, block_per_job):

    # Inaccurate and deprecated :( fix later
    # inaccurate because certain time chunks get tested more often than others
    block_check = 0
    blocks = {}

    def add_block(block_time):
        blocks[block_time] = random.randrange(4000, 500000)

    # refactor this garbage Lmao
    # ask math stack exchange
    while block_check < block_per_job:
        block = random.randrange(100, max_burst)
        if block < act_burst:
            add_block(block)
        block_check += 1

    final_block_chance = block_per_job % 1
    if random.random() < final_block_chance:
        add_block(random.randrange(100, act_burst))

    return blocks


def main():
    random.seed(CONFIG.random_seed)
    cpu: CpuState = CpuState()
    sched = SchedulerMQFS() # replace this with any scheduler object within Schedulers
    cpu.current_quantum = sched.time_quantum
    gen_arrivals()
    gen_io_events()

    # trigger io from within the process
    # if the process hits a certain burst time remaining
    # trigger IO interrupt

    tick = 0
    while tick < CONFIG.runtime_microseconds or cpu.current_job is not None:

        # Handle Interrupts
        if len(cpu.interrupt_queue) > 0:
            inter_job = cpu.interrupt_queue.pop(0)
            inter_job.handler(*inter_job.args)
            cpu.work_counter = inter_job.latency
            cpu.current_quantum = sched.time_quantum

        # Set current process from scheduler
        cpu.current_job = sched.get_curr_process()

        # add interrupts for new events(io unblocking, new processes)
        events = EVENT_MANAGER.get_event(tick)
        if events is not None:
            for event in events:
                if event.type == CpuEvent.NEW_PROCESS:
                    # object if type is new_process is process
                    cpu.add_interrupt(sched.new_process_arrival, [event.object])
                    print(f"NEW_PROCESS: {event.object} {len(cpu.interrupt_queue)}")

                elif event.type == CpuEvent.IO_UNBLOCK:
                    # object if type is io_unblock is process
                    cpu.add_interrupt(sched.io_unblock, [event.object])
                    print(f"IO_UNBLOCK: {event.object}")

                elif event.type == CpuEvent.IO_BLOCK:
                    cpu.add_interrupt(sched.io_block, [event.object])
                    print(f"IO_BLOCK: {event.object}")

                else:
                    print(f"Event Type: {event.type} Not Recognized")
            tick += 1
            continue

        # Simulates Time for internal cpu work
        # interrupt handling, cache misses(maybe)
        if cpu.work_counter > 0:
            cpu.work_counter -= 1
            tick += 1
            continue

        # If the cpu has no jobs to run, idle
        if cpu.current_job is None:
            tick += 1
            cpu.idle_ticks += 1
            continue
        elif cpu.current_job.burst_time_remaining == cpu.current_job.burst_time:
            # If this is this jobs first time running log the time it spent waiting
            cpu.wtt_rolling_time_sum += tick - cpu.current_job.arrival_time

        if cpu.current_job.burst_time_remaining in cpu.current_job.io_block_dict:
            # If the job needs to block for IO add an interrupt for it
            io_wait_time = cpu.current_job.io_block_dict[cpu.current_job.burst_time_remaining]

            del cpu.current_job.io_block_dict[cpu.current_job.burst_time_remaining]

            # Add an interrupt to block the process
            cpu.add_interrupt(sched.io_block, [cpu.current_job])
            # Add an event that triggers and interrupt to unblock the process
            EVENT_MANAGER.add_event(CpuEvent(CpuEvent.IO_UNBLOCK, cpu.current_job), tick + io_wait_time)
            continue

        if cpu.current_quantum > 0:
            cpu.current_quantum -= 1
        else:
            # If the current jobs quantum is emptied add an interrupt
            cpu.add_interrupt(sched.quantum_exhausted, [cpu.current_job])
            print(f"P{cpu.current_job.id}: Quantum Exhausted Time:{str(tick).zfill(8)}")

        if cpu.current_job.burst_time_remaining > 0:
            cpu.current_job.burst_time_remaining -= 1
        else:
            # If the current job is finished add an interrupt.
            cpu.add_interrupt(sched.process_exit, [cpu.current_job])
            print(f"P{cpu.current_job.id}: Process Exited Time:{str(tick).zfill(8)}")
            cpu.last_exit_tick = tick
            cpu.tut_rolling_time_sum += tick - cpu.current_job.arrival_time

        tick += 1
    print_metrics(cpu)


def print_metrics(cpu_state: CpuState):
    """
    CPU UTILIZATION - idle time/ run time
    Throughput - processes completed per time unity
    Waiting Time - time in ready queue
    Turnaround Time - time of arrival to time of running queue exit
    Response Time - time of submission to time or process started
    """

    print("\n================= METRICS =================\n")
    uti = cpu_state.idle_ticks / cpu_state.last_exit_tick
    uti = 1 - uti
    uti = round_three(uti)
    print(f"UTI: {uti}%")

    runtime_seconds = Units.MiS_to_seconds(cpu_state.last_exit_tick)
    tpt = CONFIG.total_jobs / runtime_seconds
    tpt = round_three(tpt)
    print(f"TPT: {tpt} Processes per Second")

    tot_wait_time = Units.MiS_to_seconds(cpu_state.wtt_rolling_time_sum)
    wtt = tot_wait_time / CONFIG.total_jobs
    wtt = round_three(wtt)
    print(f"WTT: {wtt} Seconds")

    tot_tut_time = Units.MiS_to_seconds(cpu_state.tut_rolling_time_sum)
    tut = tot_tut_time / CONFIG.total_jobs
    tut = round_three(tut)
    print(f"TUT: {tut} Seconds")


def round_three(n: int):
    return round(n, 3)


main()
