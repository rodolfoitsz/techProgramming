
import jess.*;

class JessJava {

    static char dataJessMethod(int val1,int val2,int val3,int val4) {



       try{
 
        Rete r = new Rete();
        r.eval("(reset)");

    
        int max = Math.max(Math.max(val1,val2),Math.max(val3,val4));
       


        r.eval("(deffunction max (?a ?b ?c ?d) ( if (> ?a ?b ) then if(> ?a ?c)  ) (return ?a) )");
        r.eval("(deffunction max (?b ?a ?c ?d) ( if (> ?b ?a ?c ?d) then (return ?b)  ) )");
        r.eval("(deffunction max (?c ?a ?b ?d) ( if (> ?c ?a ?b ?d) then (return ?c)  ) )");
        r.eval("(deffunction max (?d ?a ?b ?c) ( if (> ?d ?a ?b ?c) then (return ?d)  ) )");
        
       Value a = r.eval("(max 80 50 1 2)");
       Value b = r.eval("(max 80 50 1 2)");
       Value c = r.eval("(max 80 50 1 2)");
       Value d = r.eval("(max 80 50 1 2)");
       // System.out.println(max);
        System.out.println("the max "+a);
        System.out.println("the max "+b);
        System.out.println("the max "+c);
        System.out.println("the max "+d);


     //  r.eval("(max 3 5) ");

      /**  r.eval("(defrule r1 (hecho a) (hecho b) (hecho c) => (assert (tienes gripa)))");
        r.eval("(assert (hecho a))");
        r.eval("(assert (hecho b))");
        r.eval("(assert (hecho c))");       
         
        r.eval("(run)");
        r.eval("(facts)");**/
    } catch (Exception e ){

   System.out.println("This is the exception "+e);

    }

  
       return 'C';
    }

}
