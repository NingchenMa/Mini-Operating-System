1. I used the mimi.cs.mcgilll.ca
2. Here I assume that the individual script file will have at most [40 lines] of instructions.
3. When a script finishes its exection (for the exec command), its PCB will be removed from the ready queue, and its [backing-store file] will also be deleted from the BackingStore!
   This functionality is implemented in the [kernel.c] -> [int removeHeadNode()].
4. Files stored in the BackingStore are named by their pid. (That is, pid=1 -> 1.txt, pid=2 -> 2.txt, etc.)