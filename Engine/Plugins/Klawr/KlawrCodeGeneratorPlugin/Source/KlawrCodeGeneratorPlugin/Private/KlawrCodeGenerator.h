//-------------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2014 Vadim Macagon
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//-------------------------------------------------------------------------------
#pragma once

namespace Klawr {

class FCodeFormatter;

/**
 * Generates a C# wrapper class for each scriptable UE4 class.
 *
 * The generated C# class encapsulates a bunch of delegates that are bound to pointers to native
 * wrapper functions (that are also generated by this class).
 */
class FCodeGenerator
{
public:
	FCodeGenerator(
		const FString& InRootLocalPath, const FString& InRootBuildPath, 
		const FString& InOutputDirectory, const FString& InIncludeBase
	);

	void ExportClass(
		UClass* Class, const FString& SourceHeaderFilename, const FString& GeneratedHeaderFilename,
		bool bHasChanged
	);

	void FinishExport();

	static FString GetPropertyCPPType(const UProperty* Property);

	/** Check if the property type is a struct that can be used for interop. */
	static bool IsStructPropertyTypeSupported(const UStructProperty* Property);

private:
	static const FName Name_Vector2D;
	static const FName Name_Vector;
	static const FName Name_Vector4;
	static const FName Name_Quat;
	static const FName Name_Transform;
	static const FName Name_LinearColor;
	static const FName Name_Color;

	static const FString ClrHostManagedAssemblyName;
		
	/** Path where generated script glue goes **/
	FString GeneratedCodePath;
	/** Local root path **/
	FString RootLocalPath;
	/** Build root path - may be different to RootBuildPath when building remotely. **/
	FString RootBuildPath;
	/** Base include directory */
	FString IncludeBase;
	/** All generated C++ script header filenames. */
	TArray<FString> AllScriptHeaders;
	/** All the generated C# wrapper class filenames. */
	TArray<FString> AllManagedWrapperFiles;
	/** Engine source header filenames for all exported classes. */
	TArray<FString> AllSourceClassHeaders;
	/** Classes for which native wrappers were generated. */
	TArray<const UClass*> ClassesWithNativeWrappers;
	TArray<const UClass*> AllExportedClasses;

	static bool CanExportClass(const UClass* Class);
	static bool CanExportProperty(const UClass* Class, const UProperty* Property);
	static bool CanExportFunction(const UClass* Class, const UFunction* Function);

	/** Generate a .csproj for the C# wrapper classes. */
	void GenerateManagedWrapperProject();
	/** Build the generated .csproj of C# wrapper classes. */
	void BuildManagedWrapperProject();
	/** Create a 'glue' file that merges all generated script files */
	void GlueAllNativeWrapperFiles();
	
	/** Check if a property type is supported */
	static bool IsPropertyTypeSupported(const UProperty* Property);
	/** Check if the property type is a pointer. */
	static bool IsPropertyTypePointer(const UProperty* Property);

	void WriteToFile(const FString& Path, const FString& Content);
	FString RebaseToBuildPath(const FString& Filename) const;
};

} // namespace Klawr
