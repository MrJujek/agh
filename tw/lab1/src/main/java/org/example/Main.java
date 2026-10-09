package org.example;

public class Main {
    static void main() throws InterruptedException {
//        first();

        Thread c1 = new Thread(new Cowboy(5));
        Thread c2 = new Thread(new Cowboy(7));
        Thread c3 = new Thread(new Cowboy(9));
        Competition competition = new Competition();
        competition.register(c1);
        competition.register(c2);
        competition.register(c3);
        competition.startCompetition();

    }

    private static void first() throws InterruptedException {
        Thread[] countdowns = {
                new Thread(new Countdown(5), "1"),
                new Thread(new Countdown(7), "2"),
                new Thread(new Countdown(9), "3")
        };
        for (Thread countdown : countdowns) {
            System.out.println(countdown.getState());
            countdown.start();
        }

        for (Thread countdown : countdowns) {
            countdown.join();
        }

        for (Thread countdown : countdowns) {
            System.out.println(countdown.getState());
        }
    }
}
