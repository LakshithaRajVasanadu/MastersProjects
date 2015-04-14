

import java.io.IOException;
import java.util.Iterator;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
public class Reducer1 extends MapReduceBase implements Reducer<Text,Text,Text,IntWritable>
{

	public void reduce(Text key, Iterator<Text> values, OutputCollector<Text,IntWritable> output,
			Reporter reporter) throws IOException
	{

		String a = "";

		boolean first = true;
                int count = 1;

		while (values.hasNext()) {

                      String b=values.next().toString();
			if(!first) {
                             
                        if(!(a.contains(b))){
				a += ", ";
                               count++;
                        }

			}

			first = false;
                        if(!(a.contains(b)))
			a += b;
		}

	//	output.collect(key, new Text(a));
		output.collect(key,new IntWritable( count));

}
}
