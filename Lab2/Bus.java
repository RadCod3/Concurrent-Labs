public class Bus implements Runnable {
    private final BusStop busStop;

    public Bus(BusStop busStop) {
        this.busStop = busStop;
    }

    @Override
    public void run() {
        try {
            busStop.busArrives();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
//            System.out.println("Bus thread interrupted.");
        }
    }
}