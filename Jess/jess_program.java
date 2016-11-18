
import jess.*;

class Jess {

    public static void main(String[] args) {
  
        Rete r = new Rete();
        r.eval("(reset)");
        r.eval("(defrule r1 (hecho a) (hecho b) (hecho c) => (assert (tienes gripa)))");
        r.eval("(assert (hecho a))");
        r.eval("(assert (hecho b))");
        r.eval("(assert (hecho c))");       
         
        r.eval("(run)");
        r.eval("(facts)");
    }

}
