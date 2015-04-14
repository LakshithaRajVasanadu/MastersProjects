

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
import org.apache.hadoop.util.*;
public class Driver extends Configured implements Tool {
public int run(String[] args) throws Exception
{
JobConf conf = new JobConf(getConf(), Driver.class);
JobConf conf1 = new JobConf(getConf(), Driver.class);
conf.setJobName("SpamCount");
conf1.setJobName("HamCount");

conf.setOutputKeyClass(Text.class);
conf1.setOutputKeyClass(Text.class);
conf.setOutputValueClass(IntWritable.class);
conf1.setOutputValueClass(IntWritable.class);

conf.setMapperClass(Mapper1.class);
conf1.setMapperClass(Mapper1.class);
conf.setReducerClass(Reducer1.class);
conf1.setReducerClass(Reducer1.class);

FileInputFormat.addInputPath(conf, new Path(args[0]));
FileInputFormat.addInputPath(conf1, new Path(args[2]));
FileOutputFormat.setOutputPath(conf, new Path(args[1]));
FileOutputFormat.setOutputPath(conf1, new Path(args[3]));

conf.setNumReduceTasks(1);
conf1.setNumReduceTasks(1);
JobClient.runJob(conf);
JobClient.runJob(conf1);
 
return 0;
}

public static void main(String[] args) throws Exception
{
int res = ToolRunner.run(new Configuration(), new Driver(), args);
System.exit(res);
}
}
