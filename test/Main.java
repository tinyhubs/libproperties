import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.*;


public class Main
{
    static class Pair
    {
        String key;
        String val;
    }

    public static void main(String[] args) throws IOException
    {
        InputStream is   = new FileInputStream(args[0]);
        Properties  prop = new Properties();
        prop.load(is);
        
        //System.getenv()

        List<Pair> items = new ArrayList<Pair>();
        Enumeration<String> keys = (Enumeration<String>) prop.propertyNames();
        while (keys.hasMoreElements())
        {
            Pair item = new Pair();
            item.key = keys.nextElement();
            item.val = prop.getProperty(item.key);
            items.add(item);
        }
        is.close();


        items.sort(Comparator.comparing(o -> o.key));

        for (Pair item : items)
        {
            System.out.print("[");
            System.out.print(item.key);
            System.out.print("]=[");
            System.out.print(item.val);
            System.out.print("]\r\n");
        }
    }
}
