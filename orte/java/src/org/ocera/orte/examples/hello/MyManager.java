import org.ocera.orte.*;
import org.ocera.orte.types.*;

public class MyManager {

  static {
    System.loadLibrary("jorte");
  }

  public static void main(String[] args) {
    Manager manager = new Manager();

    while(true) {
      try {
        Thread.sleep(1000);
      }
      catch(Exception e) {
        e.printStackTrace();
      }
    }
  }

}
