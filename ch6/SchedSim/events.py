class CpuEvent:
    _id = 0
    IO_UNBLOCK = 1
    IO_BLOCK = 2
    NEW_PROCESS = 3
    # Types: "IO_UNBLOCK", "NEW_PROCESS"

    def __init__(self, event_type, event_object):
        CpuEvent._id += 1
        self.id = CpuEvent._id
        self.type: int = event_type
        self.object = event_object

    def __eq__(self, other):
        return other.id == self.id


class EventManager:

    def __init__(self):
        self.event_dict = {}

    def add_event(self, event: CpuEvent, time):
        if time in self.event_dict:
            self.event_dict[time].append(event)
        else:
            self.event_dict[time] = [event]

    def del_event(self, time):
        del self.event_dict[time]

    def get_event(self,time):
        if time in self.event_dict:
            return self.event_dict[time]
        return None


EVENT_MANAGER: EventManager = EventManager()
