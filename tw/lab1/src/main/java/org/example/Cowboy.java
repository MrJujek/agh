package org.example;

public class Cowboy implements Runnable {
    private int count;

    Cowboy(int from) {
        count = from;
    }

    @Override
    public void run() {
        try {
            while (!Thread.currentThread().isInterrupted()) {
                countDown();
                Thread.sleep(20);
            }
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    private synchronized void countDown() {
        count--;
    }
}
