#!/usr/bin/env python
import random
import subprocess
import sys


class Player:
    def __init__(self, ch):
        self.ch = ch

    def start(self, w, h):
        self.w = w
        self.h = h

    def update(self, a, b, c, game):
        pass

    def play(self, game):
        pass

    def finish(self, result):
        pass
        # print(self.ch, result)


class RLPlayer(Player):
    def __init__(self, ch):
        super().__init__(ch)

    def play(self, game):
        moves = []
        for r in range(self.h):
            for c in range(self.w):
                for k in range(4):
                    r0 = r * 2 + 1
                    c0 = c * 2 + 1
                    rr = r0 + self.dr[k]
                    cc = c0 + self.dc[k]
                    if game.grid[rr][cc] == ' ':
                        moves.append((r, c, k))
        random.shuffle(moves)
        return moves[0]


class Binary(Player):
    def __init__(self, ch, path_to_binary):
        super().__init__(ch)
        self.path_to_binary = path_to_binary

    def start(self, w, h):
        super().start(w, h)

        self.proc = subprocess.Popen([self.path_to_binary],
                                     stdin=subprocess.PIPE,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE)
        self.proc.stdin.write(bytes(f'{self.w} {self.h} 0\n', encoding="utf-8"))
        self.proc.stdin.flush()

    def play(self, game):
        proc = self.proc

        proc.stdin.write(b'-1 0 0\n')
        proc.stdin.flush()
        line = proc.stdout.readline()

        # x = ""
        # while True:
        #     x = proc.stderr.readline()
        #     x = str(x, encoding="utf-8")
        #     if x.strip() == "fin":
        #         break
        #     sys.stderr.write("\t" + x.strip() + "\n")


        line = str(line, encoding="utf-8")
        ret = list(map(int, line.split()))
        ret[2] %= 4
        return ret

    def update(self, a, b, c, game):
        proc = self.proc
        if c == 0:
            c = 4
        proc.stdin.write(bytes(f'{a} {b} {c}\n', encoding="utf-8"))
        proc.stdin.flush()


class BadPlayer(Player):
    def __init__(self, ch):
        super().__init__(ch)
        self.dr = [1, 0, -1, 0]
        self.dc = [0, -1, 0, 1]

    def play(self, game):
        moves = []
        for r in range(self.h):
            for c in range(self.w):
                for k in range(4):
                    r0 = r * 2 + 1
                    c0 = c * 2 + 1
                    rr = r0 + self.dr[k]
                    cc = c0 + self.dc[k]
                    moves.append((r, c, k))
        random.shuffle(moves)
        return moves[0]


class RandomPlayer(Player):
    def __init__(self, ch):
        super().__init__(ch)
        self.dr = [1, 0, -1, 0]
        self.dc = [0, -1, 0, 1]

    def play(self, game):
        moves = []
        for r in range(self.h):
            for c in range(self.w):
                for k in range(4):
                    r0 = r * 2 + 1
                    c0 = c * 2 + 1
                    rr = r0 + self.dr[k]
                    cc = c0 + self.dc[k]
                    if game.grid[rr][cc] == ' ':
                        moves.append((r, c, k))
        random.shuffle(moves)
        return moves[0]


class Dots:
    def __init__(self, h, w):
        self.h = h
        self.w = w
        self.rounds = 0

        self.dr = [1, 0, -1, 0]
        self.dc = [0, -1, 0, 1]
        self.ch = ['_', '|', '_', '|']

        self.grid = [[' ' for _ in range(self.w * 2 + 1)] for ii in range(self.h * 2 + 1)]
        # print(len(self.grid), len(self.grid[0]))
        for i in range(0, len(self.grid), 2):
            for j in range(0, len(self.grid[0]), 2):
                self.grid[i][j] = '.'

        self.rounds = self.h * (self.w + 1) + self.w * (self.h + 1)

    def arbite(self, players):
        random.shuffle(players)
        n = len(players)
        now = 0
        default_result = {p.ch: self.w * self.h for p in players}
        list(map(lambda p: p.start(self.w, self.h), players))
        # self.print()
        # print(self.rounds)
        self.to_fill = 0
        for rnds in range(self.rounds):
            try:
                r, c, k = players[now].play(self)
                if not self.place(r, c, k, players[now].ch):
                    now += 1
                    now %= n
                self.current_result = {p.ch: 0 for p in players}
                symbols = [p.ch for p in players]
                for row in self.grid:
                    for elem in row:
                        if elem in symbols:
                            self.current_result[elem] += 1
                for p in players:
                    p.update(r, c, k, self)
            except RuntimeError as e:
                # print("bad: ", e)
                default_result[players[now].ch] = 0
                for p in players:
                    p.finish(default_result)
                return default_result
            # self.print()

        default_result = {p.ch: 0 for p in players}
        symbols = [p.ch for p in players]
        for row in self.grid:
            for elem in row:
                if elem in symbols:
                    default_result[elem] += 1
        for p in players:
            p.finish(default_result)
        # print("!! ", self.to_fill)
        return default_result

    def place(self, r, c, k, symbol):
        r0 = r * 2 + 1
        c0 = c * 2 + 1
        rr = r0 + self.dr[k]
        cc = c0 + self.dc[k]
        if self.grid[rr][cc] == ' ':
            self.grid[rr][cc] = self.ch[k]
            ret = False
            for fill_dr in [-2, 2, 0, 0, 0]:
                for fill_dc in [0, 0, -2, 2, 0]:
                    new_r = r0 + fill_dr
                    new_c = c0 + fill_dc
                    if new_r < 0 or new_r >= len(self.grid) or new_c < 0 or new_c >= len(self.grid[0]) or self.grid[new_r][new_c] != ' ':
                        continue
                    if all(map(lambda i: self.grid[new_r + self.dr[i]][new_c + self.dc[i]] != ' ', range(4))):
                        self.grid[new_r][new_c] = symbol
                        self.to_fill += 1
                        ret |= True
            return ret
        else:
            raise RuntimeError("bad move")

    def print(self):
        for row in self.grid:
            for elem in row:
                print(elem, end='')
            print()
        print()
        print()


def main():
    n = int(sys.argv[1])
    dots = Dots(n, n)

    p1 = RandomPlayer("r")
    p2 = Binary("x", "/home/farnasirim/workspace/public/dots-and-lines-box/decision")

    dots.arbite([p1, p2])


if __name__ == "__main__":
    main()
