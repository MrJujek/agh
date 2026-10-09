package org.example;

import java.util.ArrayList;

public class Competition {
    private ArrayList<Thread> competitors;
    private Thread winner = null;
    private int count = 0;


    Competition() {
        competitors = new ArrayList<>();
    }

    public void register(Thread competitor) {
        competitors.add(competitor);
    }

    synchronized boolean tryWin(Thread candidate) {
        if (winner != null) {
            return false;
        }

        this.winner = candidate;

        for (Thread competitor : competitors) {
            competitor.interrupt();
        }

        return true;
    }

    public void startCompetition() {
        for (Thread competitor : competitors) {
            competitor.start();
        }
    }
}
