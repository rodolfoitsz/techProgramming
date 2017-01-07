class JavaProgram {


    public static String removeCommas(String str) {
       

        System.out.println(str); 



        int istart = str.indexOf("?"); 

        String subString1= str.substring(istart+1 , str.length());

       // System.out.println(subString1);

        int iend = subString1.indexOf("?"); 

        String subString2= subString1.trim().substring(0 , iend);
              

        System.out.println("\ncadena original recibida \n "+subString2 +"\n");

     
        str = subString2.trim().replace(",","");

         System.out.println("\ncadena a procesar \n "+str+"\n");

      

        return str;// Display the string.
    }


}