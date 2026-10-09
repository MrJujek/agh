package org.example;

public class Countdown extends Thread {
    private final int from;

    Countdown(int from) {
        this.from = from;
    }

    @Override
    public void run() {
        for (int i = from; i >= 0; i--) {
            System.out.printf("%s: %d%n", Thread.currentThread().getName(), i);
        }
    }
}
