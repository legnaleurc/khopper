#ifndef FACTORY_HPP
#define FACTORY_HPP

namespace {

	template<
		class AbstractProduct,
		typename IdentifierType,
		typename ProductCreator,
		template< typename, class > class FactoryErrorPolicy = DefaultFactoryError
	>
	class Factory : public FactoryErrorPolicy< IdentifierType, AbstractProduct > {
		public:
			bool Register( const IdentifierType & id, ProductCreator creator ) {
				return _associations_.insert( AssocMap::value_type( id, creator ) ).second;
			}
			bool Unregister( const IdentifierType & id ) {
				return _associations_.erase( id ) == 1;
			}
			AbstractProduct * CreateObject( const IdentifierType & id ) {
				typename AssocMap::const_iterator i = _associations_.find( id );
				if( i != _associations_.end() ) {
					return ( i->second )();
				}
				return OnUnknownType( id );
			}
		private:
			typedef std::map< IdentifierType, AbstractProduct > AssocMap;
			AssocMap _associations_;
	};

	template< class IdentifierType, class ProductType >
	class DefaultFactoryError {
		public:
			class Exception {
				public:
					void SetId( const IdentifierType & unknownId ) {
						_unknownId_ = unknownId;
					}
					const IdentifierType GetId() {
						return _unknownId_;
					}
				private:
					IdentifierType _unknownId_;
			};
		protected:
			ProductType * OnUnknownType( const IdentifierType & id ) {
				Error< Exception > e( "Unknown object type passed to Factory." );
				e.SetId( id );
				throw e;
			}
	};

}

#endif
