import random



def main():

    page_refs = [random.randint(0,100) for x in range(0,10000)]

    slots_to_calc = [1,2,3,4,5,6,7]

    frames_space = int(input("Frames available: "))

    calc_page_faults(page_refs,frames_space)
    """
    for frame_count in slots_to_calc:
        calc_page_faults(page_refs,frame_count)
    """

def calc_page_faults(page_refs,frame_count):
    FIFO(page_refs,frame_count)
    LRU(page_refs,frame_count)
    OPT(page_refs,frame_count)



def FIFO(page_refs,frame_count):
    frames = [None for x in range(0,frame_count)]
    faults = 0
    for ref in page_refs:
        if ref not in frames:
            faults += 1
            frames.append(ref)
            frames.pop(0)
    print(f"FIFO: \nFinal State:{frames}\nFaults:{faults}")

def LRU(page_refs,frame_count):
    frames = [None for x in range(0,frame_count)]

    faults = 0
    for ref in page_refs:
        if ref not in frames:
            faults += 1
            frames.append(ref)
            frames.pop(0)
        else:
            frames.remove(ref)
            frames.append(ref)
    print(f"LRU: \nFinal State:{frames}\nFaults:{faults}")

def OPT(page_refs,frame_count):

    frames = [None for x in range(0,frame_count)]

    faults = 0
    position = 0
    
    for ref in page_refs:
        if ref not in frames:
            faults += 1
            worst_frame = get_worst_frame(position,ref,page_refs,frames)
            frames.remove(worst_frame)
            frames.append(ref)
        position += 1
    print(f"OPT: \nFinal State:{frames}\nFaults:{faults}")


def get_worst_frame(position,ref, page_refs,frames):
    worst_frame_ind = -9000
    worst_frame = None
    for ref in frames:
        if ref not in page_refs[position:]:
            return ref
 
        ind = page_refs.index(ref,position)
        
        if ind > worst_frame_ind:
            worst_frame_ind = ind
            worst_frame = ref
    return worst_frame
        
    

main()
