
import java.util.ArrayList;
import java.io.IOException;
import java.util.StringTokenizer;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
public class Mapper1 extends MapReduceBase implements Mapper<LongWritable,Text,Text, Text>
{

	//private final static IntWritable one = new IntWritable(1);
	private Text word = new Text();
	private Text location = new Text();

	public void map(LongWritable key, Text value, OutputCollector<Text,Text> output, Reporter reporter) throws IOException
	{
		
		   FileSplit fileSplit = (FileSplit)reporter.getInputSplit();
		   String fileName = fileSplit.getPath().getName();
		   location.set(fileName);
		   
		String line = value.toString();
                    line = line.toLowerCase();
                    line = line.replaceAll("<.*?>", "");
                    line = line.replaceAll("[^a-z \\s]", ""); 
                    line = line.trim();
                    
          
		StringTokenizer tokenizer = new StringTokenizer(line);
		/*
		   while (tokenizer.hasMoreTokens())
		   {
		   word.set(tokenizer.nextToken());

		   output.collect(word,location);
		   }*/


		ArrayList<String> b = new ArrayList<String>();

		String a = "";

		while (tokenizer.hasMoreTokens()) {

			String buffer = tokenizer.nextToken();


                               b.add(buffer);

			
		}

		for(int i = 0; i < b.size(); i++) {

			output.collect(new Text(b.get(i)),location);

		}
	}
}
