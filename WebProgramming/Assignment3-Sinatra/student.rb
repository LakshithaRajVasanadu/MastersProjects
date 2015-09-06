require 'dm-core'
require 'dm-migrations'

class Student
  include DataMapper::Resource
  property :id, Serial
  property :first_name, String
  property :last_name, String
  property :student_id, String, :required=>true, :unique=>true
  property :grade, Integer
  property :sex, String
  property :dob, Date
 
  def dob=date
    super Date.strptime(date, '%m/%d/%Y')
  end
end

configure do
  enable :sessions
  set :username, 'admin'
  set :password, 'a'
end

DataMapper.finalize

get '/students' do
  @students = Student.all
  if session[:admin]== true
    slim :students
  else
    redirect to('/login')
  end
end

get '/students/new' do
  halt slim(:error) unless session[:admin]
  @student = Student.new
  slim :new_student
end

get '/students/:id' do
  halt slim(:error) unless session[:admin]
  @student = Student.get(params[:id])
  slim :show_student
end

get '/students/:id/edit' do
  halt slim(:error) unless session[:admin]
  @student = Student.get(params[:id])
  slim :edit_student
end

post '/students' do  
  halt slim(:error) unless session[:admin]
  student = Student.create(params[:student])
  redirect to("/students/#{student.id}")
end

put '/students/:id' do
  halt slim(:error) unless session[:admin]
  student = Student.get(params[:id])
  student.update(params[:student])
  redirect to("/students/#{student.id}")
end

delete '/students/:id' do
  halt slim(:error) unless session[:admin]
  Student.get(params[:id]).destroy
  redirect to('/students')
end
