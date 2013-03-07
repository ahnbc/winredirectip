/*
 * Created by SharpDevelop.
 * User: nowind
 * Date: 2013/3/7
 * Time: 12:36
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Xml;
using System.IO;
using System.Text;
namespace mslogin
{
	/// <summary>
	/// Description of LangString.
	/// </summary>
	public class LangString
	{
		XmlDocument xmlDoc= new XmlDocument();
		string lang;
		public LangString(string langname)
		{
			xmlDoc.Load("lang.xml");
			lang=langname;
			
		}
		public string get(string nodename)
		{
			XmlElement root=xmlDoc.DocumentElement;
			StringBuilder sb=new StringBuilder();
			sb.Append("/LangString/");
			sb.Append(nodename);
			XmlNode node=root.SelectSingleNode(sb.ToString());
			
			XmlNode langnode=null;
			try{
				langnode=node.Attributes[lang];}
			catch
			{return "Unknown";}
			return langnode.Value;
		}
	}
}
