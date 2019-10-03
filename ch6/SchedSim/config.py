
import json


class TesterConfig:

    def __init__(self):
        with open("config.json") as f:
            config = json.load(f)
        self.runtime_microseconds = config["runtime_seconds"] * 1000000
        self.interrupt_latency_micsec = config["interrupt_latency_micsec"]
        self.cs_latency = config["context_switch_latency_micsec"]
        self.random_seed = config["random_seed"]
        self.jobs_per_second = config["jobs_per_second"]
        self.total_jobs = int(self.jobs_per_second * (self.runtime_microseconds//1000000))
        self.job_to_runtime_ratio = config["job_to_runtime_ratio"]
        self.avg_io_block_per_sec = config["avg_io_block_per_sec"]
        self.io_block_time_micsec = config["io_block_time_micsec"]

CONFIG: TesterConfig = TesterConfig()