import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Properties;

public class Main
{
    public static void main(String[] args) throws IOException
    {
        InputStream is   = new FileInputStream("F:\\dev\\github.com\\libbylg\\libproperties\\test\\1.properties");
        Properties  prop = new Properties();
        prop.load(is);

        Enumeration<String> keys = (Enumeration<String>) prop.propertyNames();
        while (keys.hasMoreElements())
        {
            String key = keys.nextElement();
            String val = prop.getProperty(key);
            System.out.print("[");
            System.out.print(key);
            System.out.print("]=[");
            System.out.print(val);
            System.out.print("]\n");
        }

        is.close();
    }
}
