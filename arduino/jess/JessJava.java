import jess.*;
import java.util.Arrays;
import java.io.StringWriter;


class JessJava {

    static char dataJessMethod(int arrayValues[]) {
  StringWriter sw = new StringWriter();

try{

       Rete r = new Rete();
        r.eval("(reset)");


        r.eval("(defrule forward (_for)  => (printout t \"go_forward F\n \"))");
        r.eval("(defrule left (_left)  => (printout t \"turn_left L\n \" ) )");
        r.eval("(defrule right (_right)  => (printout t \"turn_right R\n \"  ))");
        r.eval("(defrule back (_back)  => (printout t \"go_back B\n \") )");

     
    // Connect the "t" router to the StringWriter
    r.addOutputRouter("t", sw);

 
     final  int val1 = arrayValues[0];
     final  int val2 = arrayValues[1];
     final  int val3 = arrayValues[2];
     final  int val4 = arrayValues[3];

 Arrays.sort( arrayValues );

 int biggestNumber =arrayValues[3];

System.out.println("the biggest number"+biggestNumber);

 if(biggestNumber == val1){
   r.eval("(assert (_for))");

 }else if (biggestNumber== val2){

r.eval("(assert (_left))");

 }else if(biggestNumber==val3){
  r.eval("(assert (_right))");


 }else {
  r.eval("(assert (_back))");

 }

  r.eval("(run)");


  System.out.println(sw);

 //r.eval("(facts)");


}catch(Exception e){

 System.out.println("This is the exception "+e);

}
 


       

   /*    try{
 
        Rete r = new Rete();
        r.eval("(reset)");

    
        int max = Math.max(Math.max(val1,val2),Math.max(val3,val4));
       


        r.eval("(deffunction max (?a ?b ?c ?d) ( if (> ?a ?b ?c ?d) then a  )");
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
        r.eval("(facts)");
    } catch (Exception e ){

   System.out.println("This is the exception "+e);

    }*/

  
       return sw.toString().charAt(sw.toString().trim().length() -1 );
    }

}
