class Queue:
    def __init__(self):
        self.ins = []
        self.out = []

    def put(self, value):
        self.ins.append(value)

    def get(self):
        if len(self.out) == 0:
            while len(self.ins) > 0:
                self.out.append(self.ins.pop(-1))

        return self.out.pop(-1) if len(self.out) > 0 else None

